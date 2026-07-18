// floason (C) 2026
// Licensed under the MIT License.

// Incorporates the IEvent interface for event signalling.

#pragma once

#include <string>
#include <unordered_map>

#include "nuke.hpp"

namespace nuke
{

class Event : public IEvent
{
public:
    // Create a new event object.
    Event(const char* name);

public:
    // Get the event name.
    virtual const char* GetName() override;

    // Get a pointer parameter.
    virtual void* GetPointer(const char* name) override;

    // Set a pointer parameter.
    virtual bool SetPointer(const char* name, void* ptr, bool overwrite = false) override;

private:
    union ParameterType
    {
        void* ptr;
    };

    std::string name_;
    std::unordered_map<std::string, ParameterType> params_;
};

}   // namespace nuke