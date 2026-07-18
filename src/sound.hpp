// floason (C) 2026
// Licensed under the MIT License.

// Base sound class which incorporates the ISound interface.

#pragma once

#include <memory>
#include <SDL3_mixer/SDL_mixer.h>

#include "nuke.hpp"
#include "nuke_sound.hpp"

namespace nuke
{

class Sound : public ISound, public IEventListener, public Updatable
{
public:
    // Load an existing sound file into memory.
    Sound(const char* path);

    // Create a new sound instance from a raw PCM audio buffer, using 32-bit
    // float signed samples. This buffer must exist in memory for the entire 
    // lifetime of this sound instance!
    Sound(const void* data, size_t len, int channels, int frequency);

    // Create a new sound instance using the same audio as another. The base
    // sound instance must remain alive for the entire duration of this
    // instance!
    Sound(const Sound& other);

    // Destroy the sound instance's MIX_Audio* instance, if owned.
    virtual ~Sound() override;

// ISound
public:
    // Has the sound instance loaded successfully?
    virtual const bool Loaded() const override;

    // Get the number of channels of the sound.
    virtual const int GetChannels() const override;
    
    // Get the frequency of the sound.
    virtual const int GetFrequency() const override;

    // Get the duration of the sound.
    virtual float GetDuration() override;

    // Get the volume of the sound (default: 1.0).
    virtual float GetVolume() override;
    
    // Set the volume of the sound.
    virtual void SetVolume(float volume) override;

    // Get the current track playback position.
    virtual float GetPlaybackPosition() override;

    // Set the track playback position.
    virtual void SetPlaybackPosition(float pos) override;

    // Get the playback speed of the sound (default: 1.0).
    virtual float GetPlaybackSpeed() override;

    // Set the playback speed of the sound.
    virtual void SetPlaybackSpeed(float speed) override;
    
    // Play the sound.
    virtual bool Play(bool loop = false, bool resume = false, float fade_in = 0.f) override;
    
    // Is the sound playing?
    virtual bool IsPlaying() override;

    // Stop the sound.
    virtual bool Stop(float fade_out = 0.f) override;
    
    // Destroy the sound instance upon stopping.
    virtual void DestroyOnFinish() override;

    // Set a parent entity if this sound instance should be played from a
    // given entity, rather than it be a global sound. Because the entity
    // assumes ownership of this sound instance, this sound will be
    // destroyed upon entity deletion.
    virtual void SetParentEntity(ICollideable* collideable) override;

    // Set the max falloff distance for the audio, given it has a parent
    // entity. Returns false if a parent entity is not set.
    virtual bool SetMaxDistance(float dist) override;

    // Get the path of the loaded sound, if applicable. This is only valid
    // so long as the sound is alive.
    virtual const char* GetLoadedPath() const override;

// IEventListener
public:
    // Called when an event is signalled.
    virtual void OnSignalEvent(IEvent* event) override;

private:
    // Create a new track after creating a MIX_Audio* instance.
    bool createTrack();

    // Adjust audio attenuation per-tick if bound to a physics context.
    void adjustAudioAttenuation();

    // Toggle whether this sound instance is listening for collideable
    // deletion.
    void toggleCollideableRemovalListen();

private:
    using unique_track = std::unique_ptr<MIX_Track, decltype(&MIX_DestroyTrack)>;
    unique_track track_;

    MIX_Audio* audio_;
    bool owns_audio_        = true;

    std::string path_;
    int channels_;
    int frequency_;
    bool loaded_            = true;

    float volume_               = 1.f;
    float playback_speed_       = 1.f;

    // The attributes below are used to modify a sound instance's volume,
    // panning and pitch given that the sound is parented to a collideable's
    // physics context.
    friend void adjust_sound_attenuation(void* userdata);
    float gain_                                 = 1.f;
    float inv_log_max_                          = 0.2f; // Assumes an approximate max distance of 100,000 
                                                        // pixels ~ 1/log10(100,000 + 1).
    float pitch_multiplier_                     = 1.f;
    PhysicsContext* parent_                     = nullptr;

    // Used for Doppler effect calculations. This is just a trial-and-error
    // constant!
    static constexpr float speed_of_sound       = 135000.f;
};

}   // namespace nuke