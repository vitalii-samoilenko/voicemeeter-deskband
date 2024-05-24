#pragma once

#ifdef NDEBUG
#include "shaders/pixel.Release.h"
#include "shaders/vertex.Release.h"
#else
#include "shaders/pixel.Debug.h"
#include "shaders/vertex.Debug.h"
#endif