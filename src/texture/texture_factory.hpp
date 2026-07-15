// floason (C) 2026
// Licensed under the MIT License.

/* 
 * This header defines the REGISTER_TEXTURE() macro, which can be called in
 * the source files for final texture classes to register the class with its
 * designated texture name, allowing it to be created through 
 * Engine::PrecacheTexture().
 * - REGISTER_TEXTURE(NAME, CLASS): register a new factory instance for the texture class.
 *   It will accept zero parameters.
 *   . E.g: REGISTER_TEXTURE("texture_rect", TextureRect);
 * - REGISTER_TEXTURE(NAME, PARAM, CLASS): register a new factory instance for the texture
 *   class. Instantiating an instance of this texture class requires one parameter, with
 *   its type specified by the PARAM macro.
 *   . E.g: REGISTER_TEXTURE("texture_image", const char*, TextureImage);
 * - REGISTER_TEXTURE(NAME, std::tuple<T1, T2, T...>, CLASS): register a new factory
 *   for the texture class, which takes in a variadic length of parameters determined by
 *   the provided tuple. Each parameter can be provided separately when invoking
 *   IEngine::CreateRawTexture().
 *   . E.g: REGISTER_TEXTURE("texture_test", std::tuple<int, const char*>, TextureTest);
 *          engine->CreateRawTexture("texture_test", 3, "hello world");
*/

// This might just be the worst C++ code I've ever written.

// TODO in the future: replace IEngine::CreateRawTexture() public interface with single
// engine model file format that is precached?

#pragma once

#include <any>
#include <type_traits>
#include <string>
#include <tuple>
#include <unordered_map>

#include "nuke.hpp"

#define _REGISTER_TEXTURE_2(NAME, CLASS)                                                            \
    static TextureFactory<CLASS> factory_##CLASS(NAME);                                             
#define _REGISTER_TEXTURE_3(NAME, PARAM, CLASS)                                                     \
    static TextureFactory<CLASS, PARAM> factory_##CLASS(NAME);                                      
#define _REGISTER_TEXTURE_SELECT(_1, _2, _3, _4, ...) _4

#define REGISTER_TEXTURE(NAME, ...)                                                                 \
    _REGISTER_TEXTURE_SELECT(NAME, __VA_ARGS__, _REGISTER_TEXTURE_3, _REGISTER_TEXTURE_2)           \
        (NAME, __VA_ARGS__)                                                                         \

namespace nuke
{

class TextureBase;

// This is the base class for all texture factory instances, required to manage
// the dictionary for each texture factory.
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
    // a name parameter for deciding which texture factory to invoke. Optional
    // parameters are propagated in the final TextureFactory instance for each 
    // class.
    template <typename... Args>
    static ITexture* Create(const char* name, void* args)
    {
        auto factory = factories().find(name);
        if (factory == factories().end())
            return nullptr;
        return (*factory->second)(args);
    }

protected:
    virtual ITexture* operator()(void* value = nullptr) = 0;
};

// Each texture factory instance is derived from this class.
template <typename T, typename Parameter = void>
class TextureFactory : public TextureFactoryBase
{
    // TextureFactory must only generate texture instances that derive from
    // TextureBase.
    static_assert(std::is_base_of<TextureBase, T>::value, "Typename must derive from TextureBase");

    // is_tuple<C>::value - returns false on non-equivalent types.
    template <typename C>
    struct is_tuple : std::false_type { };

    // is_tuple<C>::value - returns true on equivalent types.
    template <typename... Args>
    struct is_tuple<std::tuple<Args...>> : std::true_type { };

    // Compile-time expression of is_tuple<C>.
    template <typename C>
    static inline constexpr bool is_tuple_v = 
        is_tuple<std::remove_cv_t<std::remove_reference_t<C>>>::value;

public:
    TextureFactory(const char* name) : 
        TextureFactoryBase(name)
    {
    }

protected:
    virtual ITexture* operator()(void* value) override
    {
        // Whether the texture being instantiated requires an additional parameter
        // is evaluated at compile-time, thus the designated texture class does not 
        // require both constuctors to be defined to compile successfully.
        if constexpr (std::is_same_v<Parameter, void>)
            return new T();
        
        // If a tuple is provided, propagate all its elements as separate parameters.
        else if constexpr (is_tuple_v<Parameter>)
        {
            auto call_ctor = [](auto&&... args)
            {
                return new T(std::forward<decltype(args)>(args)...);
            };
            return std::apply(call_ctor, *static_cast<Parameter*>(value));
        }

        // If a single parameter was provided, cast it and pass it standalone.
        else
            return new T(*static_cast<Parameter*>(value));
    }
};

}   // namespace nuke