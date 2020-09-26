/**
 * \file window.h
 * Abstraction for creating and managing windows.
 */
#pragma once
#include "defines.h"

//Get correct Implementation
#ifdef __EMSCRIPTEN__
#include "../src/ems/Window.h"
#else
#include "../src/win/Window.h"
#endif
