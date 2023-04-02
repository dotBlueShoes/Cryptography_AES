#pragma once

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

// RESOURCE
#include "resource.h"

// SIMPLE DATATYPES
#define block inline auto
using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

