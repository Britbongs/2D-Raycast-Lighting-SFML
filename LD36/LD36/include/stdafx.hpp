// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <fstream>
#include <sstream>

//SFML Includes
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

//Game Includes
#include "Maths\MathInclude.hpp"

#define INDEX_NONE -1

#define CHECK assert

#if defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#endif

#define STATIC_CAST(type, data) static_cast<type>(data)

using Colour = Color;

#include "DebugHelper.hpp"
