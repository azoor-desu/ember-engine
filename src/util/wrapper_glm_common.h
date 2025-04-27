// Common file for all glm wrapper .h files
// Contains common settings and includes.

#pragma once
#include "types.h"

// Feature toggles (before #include "glm/glm.hpp")

#define GLM_FORCE_EXPLICIT_CTOR

#if defined(EMB_OPTIM_LEVEL) && (EMB_OPTIM_LEVEL > 0)
#define GLM_FORCE_INLINE
#endif
#if defined(EMB_DEBUG_LEVEL) && (EMB_DEBUG_LEVEL > 0)
#define GLM_FORCE_MESSAGES
#endif

//#define GLM_FORCE_INTRINSICS // TODO: Only enable for release

#include <glm/common.hpp> // all the GLSL common functions: abs, min, mix, isnan, fma, etc.
#include <glm/exponential.hpp> // all the GLSL exponential functions: pow, log, exp2, sqrt, etc.
#include <glm/trigonometric.hpp> // all the GLSL trigonometric functions: radians, cos, asin, etc.

EMB_NAMESPACE_START

// use a custom namespace for future compatibility.
namespace math = glm;

EMB_NAMESPACE_END
