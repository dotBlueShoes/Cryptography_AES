#pragma once


// WINDOWS - Modern Style - e.g. note for reader for manifest data
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// WINDOWS
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
#include <windows.h>

// OTHER
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <inttypes.h>					// uint8_t ...
#include <fstream>

// Helpers
#include <bitset>
#include <vector>
#include <string>

// RESOURCE
#include "Resource.h"

// SIMPLE DATATYPES
#define block inline auto

using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

using byte = uint8;
using wchar = wchar_t;
using size = uint64;

// ARRAY
#include <array>
template <class T, size length>
using array = std::array<T, length>;

// PAIR
template <class T>
struct pair {
	T x, y;
};