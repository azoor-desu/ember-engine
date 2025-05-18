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
#ifdef EMB_USE_GLM

#include "matrix_macros.h"
#include "wrapper_glm_common.h"

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp> // all the GLSL matrix functions: transpose, inverse, etc.

EMB_NAMESPACE_START

// use a custom namespace for future compatibility.
namespace math = glm;

// based on glm matrix class
template<embU32 size, typename T>
using embMtx = math::mat<size, size, T, glm::packed_highp>;

EMB_NAMESPACE_END

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/

// Declare all template externs, defines found in .cpp file.
EMB_NAMESPACE_START
using embMtx2 = ember::embMtx<2, ember::embF32>;
using embMtx3 = ember::embMtx<3, ember::embF32>;
using embMtx4 = ember::embMtx<4, ember::embF32>;
EMB_NAMESPACE_END

EMB_DECLARE_MTX_TYPE_ALL(ember::embF32, F)
EMB_DECLARE_MTX_TYPE_ALL(ember::embF64, F64)

#endif // EMB_USE_GLM
