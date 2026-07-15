// floason (C) 2026
// Licensed under the MIT License.

// Classes which inherit from this class can have method calls be dispatched
// by the engine at a later time period.

#pragma once

#include <type_traits>

// Call this function macro to set the update method of an updatable.
#define SET_UPDATE(CLASS, METHOD)                                                                   \
    SetUpdateFunc(&UpdateThunk<CLASS, &CLASS::METHOD>);

namespace nuke
{

class Engine;

class Updatable
{
private:
    friend Engine;
    using UpdateFunc = void (*)(Updatable* updatable);

public:
    // Set the update pointer.
    inline void SetUpdateFunc(UpdateFunc func)
    {
        update_func_ptr = func;
    }

private:
    // Call the update method. This invokes the updatable instance's thunk,
    // which calls the actual method specified by SET_UPDATE().
    inline void Update()
    {
        update_func_ptr(this);
    }

private:
    UpdateFunc update_func_ptr = nullptr;
    float time_of_dispatch = 0.f;
};

template <class T, void(T::*Update)()>
inline void UpdateThunk(Updatable* updatable)
{
    // This thunk must be called only on objects which actually implement
    // the updatable class!
    static_assert(std::is_base_of<Updatable, T>::value, "Typename must derive from Updatable");

    (static_cast<T*>(updatable)->*Update)();
}

}   // namespace nuke