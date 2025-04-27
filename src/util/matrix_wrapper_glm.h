#pragma once

#include "types.h"

#include "wrapper_glm_common.h"

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp> // all the GLSL matrix functions: transpose, inverse, etc.

EMB_NAMESPACE_START

// based on glm matrix class
template<embU32 size, typename T>
using embMtx = math::mat<size, size, T, glm::packed_highp>;

EMB_NAMESPACE_END

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/

#define EMB_DEFINE_MTX_TYPE_INTERNAL(size, valueType) \
    template struct glm::mat<size, size, valueType, glm::packed_highp>;

#define EMB_DEFINE_MTX_TYPE(size, valueType) \
    namespace glm \
    { \
    EMB_DEFINE_MTX_TYPE_INTERNAL(size, valueType) \
    }

#define EMB_DEFINE_MTX_TYPE_ALL(valueType) \
    EMB_DEFINE_MTX_TYPE(2, valueType) \
    EMB_DEFINE_MTX_TYPE(3, valueType) \
    EMB_DEFINE_MTX_TYPE(4, valueType)

#define EMB_DECLARE_MTX_TYPE(size, valueType, valueShorthand) \
    EMB_NAMESPACE_START \
    using embMtx##size##valueShorthand = embMtx<size, valueType>; \
    EMB_NAMESPACE_END \
    namespace glm \
    { \
    extern EMB_DEFINE_MTX_TYPE_INTERNAL(size, valueType) \
    }

#define EMB_DECLARE_MTX_TYPE_ALL(valueType, valueShorthand) \
    EMB_DECLARE_MTX_TYPE(2, valueType, valueShorthand) \
    EMB_DECLARE_MTX_TYPE(3, valueType, valueShorthand) \
    EMB_DECLARE_MTX_TYPE(4, valueType, valueShorthand)

// Declare all template externs, defines found in .cpp file.
EMB_NAMESPACE_START
using embMtx2 = ember::embMtx<2, ember::embF32>;
using embMtx3 = ember::embMtx<3, ember::embF32>;
using embMtx4 = ember::embMtx<4, ember::embF32>;
EMB_NAMESPACE_END

EMB_DECLARE_MTX_TYPE_ALL(ember::embF32, F)
EMB_DECLARE_MTX_TYPE_ALL(ember::embF64, F64)
