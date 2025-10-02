#pragma once
// A SMALLER CONTAINER JUST FOR VEC 2s

#include "types.h"

// proper "wrapper" model is to wrap every single call from an extern lib to our vec class.
// [extern_wrapper] > [interface] > [engine]
// so that engine always calls a standardized API via the interface, no matter the extern impl.
// The extern_wrapper should convert native extern lib syntax into our embVec syntax.
// The interface should be able to grab any one of these extern wrappers and use it interchangably,
// maybe even using parts of different extern libs at once.

//  but im lazy so extern wrapper is just a typedef, and interface is just a simple include lmao.

#ifdef EMB_USE_GLM

#    include "macros_vec_mtx.h"
#    include "wrapper_glm_common.h"

#    include "glm/ext/vector_float2.hpp"
#    include "glm/ext/vector_int2.hpp"

EMB_NAMESPACE_START

// use a custom namespace for future compatibility.
namespace math = glm;

// based on glm vec class
template<embU32 Size, typename T>
using embVec = math::vec<Size, T, glm::packed_highp>;

EMB_NAMESPACE_END

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/

// Declare all template externs, defines found in .cpp file.
EMB_DECLARE_VEC_TYPE(2, ember::embS32, S32)
EMB_DECLARE_VEC_TYPE(2, ember::embF32, F32)

// Additional container names
EMB_NAMESPACE_START
using embCoordInt = math::vec<2, ember::embS32>;
using embCoord = math::vec<2, ember::embF32>;
EMB_NAMESPACE_END

#endif // EMB_USE_GLM
