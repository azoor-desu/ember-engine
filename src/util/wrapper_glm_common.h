// Common file for all glm wrapper .h files
// Contains common settings and includes.

#pragma once

// Feature toggles (before #include "glm/glm.hpp")

#define GLM_FORCE_EXPLICIT_CTOR

#if defined(EMB_RELEASE) //&& (EMB_OPTIM_LEVEL > 0)
#define GLM_FORCE_INLINE
#define GLM_FORCE_INTRINSICS
#endif
#if defined(EMB_DEBUG) //&& (EMB_DEBUG_LEVEL > 0)
#define GLM_FORCE_MESSAGES
#endif
