// floason (C) 2026
// Licensed under the MIT License.

#include <iostream>

#include "nuke.hpp"
#include "SDL3/SDL.h"

namespace nuke
{

// Test function.
void nuke_print_hello()
{
    std::cout << "Hello world!" << std::endl;
    std::cout << "Nuke v" << nuke::major << "." << nuke::minor << "." << nuke::patch << std::endl;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    SDL_Quit();
}

}   // namespace nuke