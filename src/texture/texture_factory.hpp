// floason (C) 2026
// Licensed under the MIT License.

/* 
 * This header defines the REGISTER_TEXTURE() macro, which can be called in
 * the source files for final texture classes to register the class with its
 * designated texture name, allowing it to be created through 
 * Engine::PrecacheTexture().
 * - REGISTER_TEXTURE(NAME, CLASS): register a new factory instance for the texture class.
 *   It will accept zero parameters.
 *   . E.g: REGISTER_TEXTURE("texture_rect", TextureRect).
 * - REGISTER_TEXTURE(NAME, PARAM, CLASS): register a new factory instance for the texture
 *   class. Instantiating an instance of this texture class requires one parameter, with
 *   its type specified by the PARAM macro.
 *   . E.g: REGISTER_TEXTURE("texture_image", const char*, TextureImage).
*/

// This might just be the worst C++ code I've ever written.

// TODO in the future: replace textures module with single texture class that loads from
// nuke texture file format?

#pragma once

#include <any>
#include <type_traits>
#include <string>
#include <unordered_map>

#include "nuke.hpp"

#define _REGISTER_TEXTURE_2(NAME, CLASS)                                                            \
    static TextureFactory<CLASS> factory_##CLASS(NAME);
#define _REGISTER_TEXTURE_3(NAME, PARAM, CLASS)                                                     \
    static TextureFactory<CLASS, PARAM> factory_##CLASS(NAME);
#define _REGISTER_TEXTURE_SELECT(_1, _2, _3, _4, ...) _4

#define REGISTER_TEXTURE(NAME, ...)                                                                 \
    _REGISTER_TEXTURE_SELECT(NAME, __VA_ARGS__, _REGISTER_TEXTURE_3, _REGISTER_TEXTURE_2)           \
        (NAME, __VA_ARGS__)

namespace nuke
{

class TextureBase;

class TextureFactoryBase
{
private:
    // Required to force the factories map to be instantiated before any call to
    // TextureFactoryBase's constructor is made!
    static inline auto& factories()
    {
        static std::unordered_map<std::string, TextureFactoryBase*> factories_;
        return factories_;
    }

public:
    TextureFactoryBase(const char* name)
    {
        factories()[name] = this;
    }

public:
    // Texture instances are instantiated though this static method, which accepts
    // a name parameter for deciding which texture factory to invoke, alongside
    // optional parameters, although only one optional parameter is currently
    // supported.
    template <typename... Args>
    static ITexture* Create(const char* name, Args&&... args)
    {
        auto factory = factories().find(name);
        if (factory == factories().end())
            return nullptr;
        return (*factory->second)(std::forward<Args>(args)...);
    }

protected:
    virtual ITexture* operator()(std::any value = std::any()) = 0;
};

template <typename T, typename parameter = void>
class TextureFactory : public TextureFactoryBase
{
    // TextureFactory must only generate texture instances that derive from
    // TextureBase.
    static_assert(std::is_base_of<TextureBase, T>::value, "Typename must derive from TextureBase");

public:
    TextureFactory(const char* name) : 
        TextureFactoryBase(name)
    {
    }

protected:
    virtual ITexture* operator()(std::any value) override
    {
        // Whether the texture being instantiated requires an additional parameter
        // is evaluated at compile-time, thus the designated texture class does not 
        // require both constuctors to be defined to compile successfully.
        if constexpr (std::is_same_v<parameter, void>)
            return new T();
        else
            return new T(std::any_cast<parameter>(value));
    }
};

}   // namespace nuke