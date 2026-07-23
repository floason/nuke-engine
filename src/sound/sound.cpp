// floason (C) 2026
// Licensed under the MIT License.

// Base sound class which incorporates the ISound interface.

#include <cstddef>
#include <cstring>
#include <SDL3_mixer/SDL_mixer.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "sound.hpp"
#include "soundapi.hpp"

namespace nuke
{

// Callback function which destroys the given audio instance when its track 
// stops.
SDLCALL void destroy_audio_on_finish(void* userdata, MIX_Track* track)
{
    delete static_cast<Sound*>(userdata);
}

// Load an existing sound file into memory.
Sound::Sound(const char* path) :
    track_(nullptr, MIX_DestroyTrack)
{
    SDL_AudioSpec spec;
    audio_ = MIX_LoadAudio(engine.sound.mixer_, path, false);
    if (audio_ == nullptr)
        goto fail_audio;

    if (!createTrack())
        goto fail_track;

    if (MIX_GetAudioFormat(audio_, &spec))
    {
        channels_ = spec.channels;
        frequency_ = spec.freq;
    }

    path_ = path;
    return;

fail_track:
    MIX_DestroyAudio(audio_);
fail_audio:
    path_ = std::string("MISSING-") + path;
    loaded_ = false;
}

// Create a new sound instance from a raw PCM audio buffer, using 32-bit
// float signed samples. This buffer must exist in memory for the entire 
// lifetime of this sound instance!
Sound::Sound(const void* data, size_t len, int channels, int frequency) :
    track_(nullptr, MIX_DestroyTrack),
    path_(""),
    channels_(channels),
    frequency_(frequency)
{
    SDL_AudioSpec spec = { SDL_AUDIO_F32, channels, frequency };
    audio_ = MIX_LoadRawAudioNoCopy(engine.sound.mixer_,
                                    data,
                                    len,
                                    &spec,
                                    false);
    if (audio_ == nullptr)
        goto fail_audio;

    if (!createTrack())
        goto fail_track;

    return;

fail_track:
    MIX_DestroyAudio(audio_);
fail_audio:
    loaded_ = false;
}

// Create a new sound instance using the same audio as another. The base
// sound instance must remain alive for the entire duration of this
// instance!
Sound::Sound(const Sound& other) :
    track_(nullptr, MIX_DestroyTrack),
    audio_(other.audio_),
    owns_audio_(false),
    path_(other.path_),
    channels_(other.channels_),
    frequency_(other.frequency_)
{
    if (!other.loaded_ || !createTrack())
        loaded_ = false;
}

// Destroy the sound instance's MIX_Audio* instance, if owned.
Sound::~Sound()
{
    if (owns_audio_ && audio_ != nullptr)
        MIX_DestroyAudio(audio_);
    toggleCollideableRemovalListen();
}

// Has the sound instance loaded successfully?
const bool Sound::Loaded() const
{
    return loaded_;
}

// Get the number of channels of the sound.
const int Sound::GetChannels() const
{
    return channels_;
}

// Get the frequency of the sound.
const int Sound::GetFrequency() const
{
    return frequency_;
}

// Get the duration of the sound.
float Sound::GetDuration()
{
    if (!loaded_)
        return 0.f;
    return MIX_GetAudioDuration(audio_);
}

// Get the volume of the sound (default: 1.0).
float Sound::GetVolume()
{
    return volume_;
}

// Set the volume of the sound.
void Sound::SetVolume(float volume)
{
    if (!loaded_)
        return;
    volume_ = volume;
    MIX_SetTrackGain(track_.get(), volume * gain_);
}

// Get the current track playback position.
float Sound::GetPlaybackPosition()
{
    if (!loaded_)
        return 0.f;
    return static_cast<float>(MIX_GetTrackPlaybackPosition(track_.get())) / frequency_;
}

// Set the track playback position.
void Sound::SetPlaybackPosition(float pos)
{
    if (!loaded_)
        return;
    MIX_SetTrackPlaybackPosition(track_.get(), static_cast<Sint64>(pos * frequency_));
}

// Get the playback speed of the sound (default: 1.0).
float Sound::GetPlaybackSpeed()
{
    return playback_speed_;
}

// Set the playback speed of the sound.
void Sound::SetPlaybackSpeed(float speed)
{
    if (!loaded_)
        return;
    playback_speed_ = speed;
    MIX_SetTrackFrequencyRatio(track_.get(), speed * pitch_multiplier_);
}

// Play the sound.
bool Sound::Play(bool loop, bool resume, float fade_in)
{
    // Refuse to play if engine audio output is disabled.
    if (!engine.output_enabled)
        return false;
    
    if (!loaded_)
        return false;

    SDL_PropertiesID options = SDL_CreateProperties();
    if (!options)
        return false;

    if (loop)
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    if (resume)
    {
        Sint64 start = MIX_GetTrackPlaybackPosition(track_.get());
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_START_FRAME_NUMBER, start);
    }
    if (fade_in > 0)
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_FRAMES_NUMBER, (fade_in * frequency_));

    bool result = MIX_PlayTrack(track_.get(), options);
    SDL_DestroyProperties(options);
    return result;
}

// Is the sound playing?
bool Sound::IsPlaying()
{
    if (!loaded_)
        return false;
    return MIX_TrackPlaying(track_.get());
}

// Stop the sound.
bool Sound::Stop(float fade_out)
{
    if (!loaded_)
        return false;
    return MIX_StopTrack(track_.get(), static_cast<Sint64>(fade_out * frequency_));
}

// Destroy the sound instance upon stopping.
void Sound::DestroyOnFinish()
{
    if (!loaded_)
        return;
    MIX_SetTrackStoppedCallback(track_.get(), &destroy_audio_on_finish, this);
}

// Set a parent entity if this sound instance should be played from a
// given entity, rather than it be a global sound. Because the entity
// assumes ownership of this sound instance, this sound will be
// destroyed upon entity deletion.
void Sound::SetParentEntity(ICollideable* collideable)
{
    if (!loaded_)
        return;

    if (collideable != nullptr)
    {
        parent_ = &collideable->GetPhysicsContext();
        engine.DispatchUpdate(this);
    }
    else
    {
        MIX_StereoGains gains = { 1.f, 1.f };
        MIX_SetTrackStereo(track_.get(), &gains);
        MIX_SetTrackGain(track_.get(), volume_);
        parent_ = nullptr;
    }

    toggleCollideableRemovalListen();
}

// Set the max falloff distance for the audio, given it has a parent
// entity. Returns false if a parent entity is not set.
bool Sound::SetMaxDistance(float dist)
{
    if (!loaded_ || parent_ == nullptr)
        return false;
    inv_log_max_ = 1.f / std::log10(dist + 1);
    return true;
}

// Get the path of the loaded sound, if applicable. This is only valid
// so long as the sound is alive.
const char* Sound::GetLoadedPath() const
{
    return path_.c_str();
}

// Called when an event is signalled.
void Sound::OnSignalEvent(IEvent* event)
{
    // TODO: this doesn't seem like a good design choice particularly as
    // it relies on SDK code to signal collideable deletion + every sound
    // is now signalled of every collideable deletion.
    //
    // use entity handles to resolve lifetime in the future?

    // The only event sound instances are bound to is collideable_removed.
    // If the physics context of the collideable is aggregated to this
    // sound instance, it must be immediately decoupled.
    if (event->GetPointer("physics_context") == static_cast<void*>(parent_))
        SetParentEntity(nullptr);
}

// Create a new track after creating a MIX_Audio* instance.
bool Sound::createTrack()
{
    if (!loaded_)
        return false;

    MIX_Track* track = MIX_CreateTrack(engine.sound.mixer_);
    if (track == nullptr)
        return false;

    SET_UPDATE(Sound, adjustAudioAttenuation);
    MIX_SetTrackAudio(track, audio_);
    track_.reset(track);
    return true;
}

// Adjust audio attenuation per-tick if bound to a physics context,
// alongside audio panning and playback speed (doppler effect).
void Sound::adjustAudioAttenuation()
{
    if (parent_ == nullptr)
        return;

    // Calculate the gain multiplier of the audio. Attenuation is calculated
    // logarithmically so as to attempt to mimic realistic audio falloff.
    Vector2 origin_from_screen = parent_->origin
                               - engine.camera_context.camera_offset 
                               - engine.camera_context.attenuation_offset;
    Vector2 normalized = origin_from_screen.Normalize();
    float dist = origin_from_screen.Length();
    gain_ = nuke::math::max(1.f - std::log10(dist + 1.f) * inv_log_max_, 0.f);
    MIX_SetTrackGain(track_.get(), volume_ * gain_);

    // Pan the audio between stereo speakers using the normalised vector.
    float angle = (normalized.x + 1.f) * nuke::math::pi::f * 0.25f;
    MIX_StereoGains gains = { std::cosf(angle), std::sinf(angle) };
    MIX_SetTrackStereo(track_.get(), &gains);

    // Calculate the playback speed multiplier of the audio based on its
    // relative offset and speed to the listener.
    Vector2 relative_velocity = parent_->velocity - engine.camera_context.doppler_velocity;
    float radial_multiplier = nuke::math::max(relative_velocity.Dot(normalized), -speed_of_sound + 1);
    pitch_multiplier_ = speed_of_sound / (speed_of_sound + radial_multiplier);
    MIX_SetTrackFrequencyRatio(track_.get(), playback_speed_ * pitch_multiplier_);

    engine.DispatchUpdate(this);
}

// Toggle whether this sound instance is listening for collideable
// deletion.
void Sound::toggleCollideableRemovalListen()
{
    if (parent_ == nullptr)
        engine.RemoveEventListener(this, "collideable_removed");
    else
        engine.AddEventListener(this, "collideable_removed");
}

}   // namespace nuke