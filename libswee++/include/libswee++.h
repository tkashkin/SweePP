#pragma once

#include <cstdint>
#include <ctime>

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

namespace sweepp
{
	struct SWEEPP Point
	{
		uint8_t x = 0;
		uint8_t y = 0;
		
		Point(uint8_t x, uint8_t y): x(x), y(y)
		{}
		
		bool operator==(const Point &rhs)
		{ return this->x == rhs.x && this->y == rhs.y; }
	};
	
	typedef SWEEPP Point Size;
}