#pragma once

#ifdef _WIN32
    #ifdef SWEEPP_EXPORT
        #define SWEEPP __declspec(dllexport)
    #else
        #define SWEEPP __declspec(dllimport)
    #endif
#else
    #define SWEEPP
#endif

#undef __min
#define __min(a, b) ((a) < (b) ? (a) : (b))

#undef __max
#define __max(a, b) ((a) > (b) ? (a) : (b))