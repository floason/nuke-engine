// floason (C) 2026
// Licensed under the MIT License.

// Interface class for event signalling. Classes can inherit from another
// interface class - IEventListener - so as to listen for any events.

#pragma once

namespace nuke
{

class IEvent
{
public:
    virtual ~IEvent() = default;

public:
    // Get the event name.
    virtual const char* GetName() = 0;

    // Get a pointer parameter.
    virtual void* GetPointer(const char* name) = 0;

    // Set a pointer parameter.
    virtual bool SetPointer(const char* name, void* ptr, bool overwrite = false) = 0;
};

class IEventListener
{
public:
    // Called when an event is signalled.
    virtual void OnSignalEvent(IEvent* event) = 0;
};

}   // namespace nuke