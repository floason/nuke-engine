// floason (C) 2026
// Licensed under the MIT License.

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