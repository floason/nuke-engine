// floason (C) 2026
// Licensed under the MIT License.

// Defines classes for game variables (which store data that can be accessible
// by both the engine and the game) and game commands (which are callable by
// both engine and game code).

#pragma once

#include <cstddef>
#include <cstring>

#include "nuke_math.hpp"

namespace nuke
{

// Both GameVar and GameCmd inherit from this.
class GameVarBase
{
public:
    enum Flags
    {
        FLAG_NONE   = 0,

        FLAG_HIDDEN = (1 << 0), // Hide this game variable from the player.
    };

protected:
    GameVarBase(const char* name, const char* description = "", Flags flags = FLAG_NONE) :
        flags_(flags)
    {
        name_ = new char[strlen(name) + 1]();
        strcpy(name_, name);

        size_t description_length = strlen(description);
        if (description_length > 0)
        {
            description_ = new char[description_length + 1]();
            strcpy(description_, description);
        }
    }

public:
    // This class should not be instantiable!
    GameVarBase() = delete;

    virtual ~GameVarBase()
    {
        if (name_ != nullptr)
            delete[] name_;
        if (description_ != nullptr)
            delete[] description_;
    }

public:
    // Is this game variable callable?
    inline bool IsCallable()
    {
        return is_cmd_;
    }

    // Get the name of this base game variable.
    inline const char* GetName()
    {
        return name_;
    }

    // Get the description of this base game variable.
    inline const char* GetDescription()
    {
        return description_;
    }

    // Get this game variable's flags.
    inline Flags GetFlags()
    {
        return flags_;
    }

    // Set this game variable's flags.
    inline void SetFlags(Flags flags)
    {
        flags_ = flags;
    }

private:
    char* name_             = nullptr;
    char* description_      = nullptr;

protected:
    bool is_cmd_            = false;
    Flags flags_            = FLAG_NONE;
};

class GameVar : public GameVarBase
{
public:
    enum TypeName
    {
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_STRING
    };

public:
    // Initialize the game variable with an integer value.
    GameVar(const char* name, 
            int value, 
            const char* description = "",
            int min = -1, 
            int max = -1,
            Flags flags = FLAG_NONE) :
        GameVarBase(name, description, flags),
        value_type_(TYPE_INT)
    {
        SetInt(value);
        value_float_default_ = value_float_;

        if (min != -1)
            SetMin(min);
        if (max != -1)
            SetMax(max);
    }

    // Initialize the game variable with a float value.
    GameVar(const char* name, 
            float value, 
            const char* description = "", 
            float min = -1.f, 
            float max = -1.f,
            Flags flags = FLAG_NONE) :
        GameVarBase(name, description, flags),
        value_type_(TYPE_FLOAT)
    {
        SetFloat(value);
        value_float_default_ = value_float_;

        if (min != -1)
            SetMin(min);
        if (max != -1)
            SetMax(max);
    }

    // Initialize the game variable with a string value.
    GameVar(const char* name, const char* value, const char* description = "", Flags flags = FLAG_NONE) :
        GameVarBase(name, description, flags),
        value_type_(TYPE_STRING)
    {
        SetString(value);
        value_string_default_ = value_string_;
    }

    virtual ~GameVar() override
    {
        if (value_string_default_ != value_string_ && value_string_default_ != nullptr)
            delete[] value_string_default_;
        if (value_string_ != nullptr)
            delete[] value_string_;
    }

public:
    // Get the type used.
    inline TypeName GetType()
    {
        return value_type_;
    }

    // Get the variable's integer value.
    inline int GetInt()
    {
        if (value_type_ != TYPE_INT)
            return 0;
        return value_float_;
    }

    // Get the variable's float value.
    inline float GetFloat()
    {
        if (value_type_ != TYPE_FLOAT)
            return 0;
        return value_float_;
    }

    // Get the variable's string pointer.
    inline const char* GetString()
    {
        if (value_type_ != TYPE_STRING)
            return nullptr;
        return value_string_;
    }

    // Set the variable's integer value.
    inline bool SetInt(int value)
    {
        if (value_type_ != TYPE_INT)
            return false;
        
        value_float_ = value;
        if (min_value_set_)
            value_float_ = nuke::math::max(static_cast<float>(value), min_value_);
        if (max_value_set_)
            value_float_ = nuke::math::min(static_cast<float>(value), max_value_);
        
        return true;
    }

    // Set the variable's float value.
    inline bool SetFloat(float value)
    {
        if (value_type_ != TYPE_FLOAT)
            return false;
        
        value_float_ = value;
        if (min_value_set_)
            value_float_ = nuke::math::max(value, min_value_);
        if (max_value_set_)
            value_float_ = nuke::math::min(value, max_value_);
        
        return true;
    }

    // Set the variable's string buffer.
    inline bool SetString(const char* buffer)
    {
        if (buffer == nullptr)
            return false;

        if (value_type_ != TYPE_STRING)
            return false;

        size_t length = strlen(buffer) + 1;
        if (length <= 1)
            return false;

        // Only re-allocate the string buffer if the new value being set is
        // larger than the previously allocated size.
        if (length > value_string_maxsize_)
        {
            // Only de-allocate value buffer if the default value pointer does
            // not currently point to it.
            if (value_string_ != nullptr && value_string_ != value_string_default_)
                delete[] value_string_;

            value_string_ = new char[length]();
            value_string_maxsize_ = length;
        }
        else
            memset(value_string_, 0, value_string_maxsize_);

        strcpy(value_string_, buffer);
        return true;
    }

    // Is the minimum value set?
    inline bool ClampsMin()
    {
        if (value_type_ == TYPE_STRING)
            return false;
        return min_value_set_;
    }

    // Get the minimum value.
    inline float GetMin()
    {
        if (value_type_ == TYPE_STRING)
            return 0.f;
        return min_value_;
    }

    // Set the minimum value.
    inline bool SetMin(float min)
    {
        if (value_type_ == TYPE_STRING)
            return false;
        min_value_ = min;
        min_value_set_ = true;
        return true;
    }

    // Stop clamping the value above the minimum value.
    inline bool StopClampingMin()
    {
        if (value_type_ == TYPE_STRING)
            return false;
        min_value_set_ = false;
        return true;
    }

    // Is the maximum value set?
    inline bool ClampsMax()
    {
        if (value_type_ == TYPE_STRING)
            return false;
        return max_value_set_;
    }

    // Get the maximum value.
    inline float GetMax()
    {
        if (value_type_ == TYPE_STRING)
            return 0.f;
        return max_value_;
    }
    
    // Set the minimum value.
    inline bool SetMax(float min)
    {
        if (value_type_ == TYPE_STRING)
            return false;
        max_value_ = min;
        max_value_set_ = true;
        return true;
    }

    // Stop clamping the value below the minimum value.
    inline bool StopClampingMax()
    {
        if (value_type_ == TYPE_STRING)
            return false;
        max_value_set_ = false;
        return true;
    }

private:
    TypeName value_type_;

    bool min_value_set_             = false;
    bool max_value_set_             = false;
    float min_value_                = 0.f;
    float max_value_                = 0.f;

    float value_float_              = 0.f;
    float value_float_default_      = 0.f;
    char* value_string_             = nullptr;
    char* value_string_default_     = nullptr;
    size_t value_string_maxsize_    = 0;
};

class GameCmd : public GameVarBase
{
public:
    using CallbackFunc                          = bool (*)(int argc, char** argv);
    using CallbackFuncNoParams                  = bool (*)();

public:
    // Initialize the game command using a callback function that accepts
    // parameters.
    GameCmd(const char* name, CallbackFunc func, const char* description = "", Flags flags = FLAG_NONE) :
        GameVarBase(name, description, flags),
        func_(func)
    {
        callback_requires_params_ = true;
        construct();
    }

    // Initialize the game command using a callback function that does not
    // accept parameters.
    GameCmd(const char* name, 
            CallbackFuncNoParams func, 
            const char* description = "", 
            Flags flags = FLAG_NONE) :
        GameVarBase(name, description, flags),
        func_no_params_(func)
    {
        construct();
    }

public:
    // Update the callback function.
    inline void UpdateCallbackFunc(CallbackFunc func)
    {
        func_ = func;
    }

    // Call the callback function without any parameters.
    inline bool Invoke()
    {
        if (callback_requires_params_)
            return func_(0, nullptr);
        return func_no_params_();
    }

    // Call the callback function.
    inline bool Invoke(int argc, char** argv)
    {
        // Given the assumption that at least one parameter was actually
        // passed, if the callback function does not accept any
        // parameters, this invokation cannot proceed.
        if (!callback_requires_params_)
            return false;
        
        return func_(argc, argv);
    }

private:
    // Called from both constructors.
    inline void construct()
    {
        is_cmd_ = true;
    }

private:
    union
    {
        CallbackFunc func_;
        CallbackFuncNoParams func_no_params_;
    };
    bool callback_requires_params_              = false;
};

}   // namespace nuke