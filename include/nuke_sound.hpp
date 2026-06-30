// floason (C) 2026
// Licensed under the MIT License.

// Interface class for a sound stream.

#pragma once

namespace nuke
{

class IEntity;

class ISound
{
public:
    virtual ~ISound() = default;

public:
    // Has the sound instance loaded successfully?
    virtual const bool Loaded() const = 0;

    // Get the number of channels of the sound.
    virtual const int GetChannels() const = 0;
    
    // Get the frequency of the sound.
    virtual const int GetFrequency() const = 0;

    // Get the duration of the sound.
    virtual float GetDuration() = 0;

    // Get the volume of the sound (default: 1.0).
    virtual float GetVolume() = 0;
    
    // Set the volume of the sound.
    virtual void SetVolume(float volume) = 0;

    // Get the current track playback position.
    virtual float GetPlaybackPosition() = 0;

    // Set the track playback position.
    virtual void SetPlaybackPosition(float pos) = 0;

    // Get the playback speed of the sound (default: 1.0).
    virtual float GetPlaybackSpeed() = 0;

    // Set the playback speed of the sound.
    virtual void SetPlaybackSpeed(float speed) = 0;

    // Play the sound.
    virtual bool Play(bool loop = false, bool resume = false, float fade_in = 0.f) = 0;

    // Is the sound playing?
    virtual bool IsPlaying() = 0;

    // Stop the sound.
    virtual bool Stop(float fade_out = 0.f) = 0;

    // Destroy the sound instance upon stopping.
    virtual void DestroyOnFinish() = 0;

    // Set a parent entity if this sound instance should be played from a
    // given entity, rather than it be a global sound. Because the entity
    // assumes ownership of this sound instance, this sound will be
    // destroyed upon entity deletion.
    virtual void SetParentEntity(IEntity* entity) = 0;

    // Set the max falloff distance for the audio, given it has a parent
    // entity. Returns false if a parent entity is not set.
    virtual bool SetMaxDistance(float dist) = 0;

    // Get the path of the loaded sound, if applicable. This is only valid
    // so long as the sound is alive.
    virtual const char* GetLoadedPath() const = 0;
};

}   // namespace nuke