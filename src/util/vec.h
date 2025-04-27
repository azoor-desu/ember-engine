#pragma once

#include "types.h"

// proper "wrapper" model is to wrap every single call from an extern lib to our vec class.
// [extern_wrapper] > [interface] > [engine]
// so that engine always calls a standardized API via the interface, no matter the extern impl.
// The extern_wrapper should convert native extern lib syntax into our embVec syntax.
// The interface should be able to grab any one of these extern wrappers and use it interchangably,
// maybe even using parts of different extern libs at once.

//  but im lazy so extern wrapper is just a typedef, and interface is just a simple include lmao.


// Pick implementation to use.
#define EMB_USE_VEC_GLM // lol move this elsewhere
#ifdef EMB_USE_VEC_GLM
#include "vec_wrapper_glm.h"
#endif
