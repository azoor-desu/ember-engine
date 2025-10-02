#pragma once

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

#    include <glm/geometric.hpp> // all the GLSL geometry functions: dot, cross, reflect, etc.
#    include <glm/vec2.hpp>
#    include <glm/vec3.hpp>
#    include <glm/vec4.hpp>

#    include <glm/common.hpp> // all the GLSL common functions: abs, min, mix, isnan, fma, etc.
#    include <glm/exponential.hpp> // all the GLSL exponential functions: pow, log, exp2, sqrt, etc.
#    include <glm/trigonometric.hpp> // all the GLSL trigonometric functions: radians, cos, asin, etc.

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
EMB_NAMESPACE_START
using embVec2 = ember::embVec<2, ember::embF32>;
using embVec3 = ember::embVec<3, ember::embF32>;
using embVec4 = ember::embVec<4, ember::embF32>;
EMB_NAMESPACE_END

EMB_DECLARE_VEC_TYPE_ALL(ember::embF32, F)
EMB_DECLARE_VEC_TYPE_ALL(ember::embF64, F64)

#endif // EMB_USE_GLM
