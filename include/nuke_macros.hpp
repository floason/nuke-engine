// floason (C) 2026
// Licensed under the MIT License.

#pragma once

#ifdef NUKE_SHARED_LIBRARY
#   ifdef _MSC_VER
#       ifdef NUKE_EXPORT
#           define NUKE_API __declspec(dllexport)
#       else
#           define NUKE_API __declspec(dllimport)
#       endif
#   else
#       define NUKE_API __attribute__((visibility("default")))
#   endif
#else
#   define NUKE_API
#endif

#define NUKE_DEFAULT_TICKRATE       60.f
#define NUKE_MAX_TICKS_PER_FRAME    10
#define NUKE_NS_PER_S               1000000000ULL