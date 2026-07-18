// floason (C) 2026
// Licensed under the MIT License.

// Incorporates the IEvent interface for event signalling.

#include <string>

#include "event.hpp"

namespace nuke
{

// Create a new event object.
Event::Event(const char* name) :
    name_(name)
{
}

// Get the event name.
const char* Event::GetName()
{
    return name_.c_str();
}

// Get a pointer parameter.
void* Event::GetPointer(const char* name)
{
    if (params_.find(name) == params_.end())
        return nullptr;
    return params_[name].ptr;
}

// Set a pointer parameter.
bool Event::SetPointer(const char* name, void* ptr, bool overwrite)
{
    if (params_.find(name) != params_.end() && !overwrite)
        return false;
    params_[name] = { .ptr = ptr };
    return true;
}

}   // namespace nuke