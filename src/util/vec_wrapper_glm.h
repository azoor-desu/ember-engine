#pragma once

#include "types.h"

#include "wrapper_glm_common.h"

#include <glm/geometric.hpp> // all the GLSL geometry functions: dot, cross, reflect, etc.
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

EMB_NAMESPACE_START

// based on glm vec class
template<embU32 Size, typename T>
using embVec = math::vec<Size, T, glm::packed_highp>;

EMB_NAMESPACE_END

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/

#define EMB_DEFINE_VEC_TYPE_INTERNAL(size, valueType) \
    template struct glm::vec<size, valueType, glm::packed_highp>;

#define EMB_DEFINE_VEC_TYPE(size, valueType) \
    namespace glm \
    { \
    EMB_DEFINE_VEC_TYPE_INTERNAL(size, valueType) \
    }

#define EMB_DEFINE_VEC_TYPE_ALL(valueType) \
    EMB_DEFINE_VEC_TYPE(2, valueType) \
    EMB_DEFINE_VEC_TYPE(3, valueType) \
    EMB_DEFINE_VEC_TYPE(4, valueType)

#define EMB_DECLARE_VEC_TYPE(size, valueType, valueShorthand) \
    EMB_NAMESPACE_START \
    using embVec##size##valueShorthand = embVec<size, valueType>; \
    EMB_NAMESPACE_END \
    namespace glm \
    { \
    extern EMB_DEFINE_VEC_TYPE_INTERNAL(size, valueType) \
    }

#define EMB_DECLARE_VEC_TYPE_ALL(valueType, valueShorthand) \
    EMB_DECLARE_VEC_TYPE(2, valueType, valueShorthand) \
    EMB_DECLARE_VEC_TYPE(3, valueType, valueShorthand) \
    EMB_DECLARE_VEC_TYPE(4, valueType, valueShorthand)

// Declare all template externs, defines found in .cpp file.
EMB_NAMESPACE_START
using embVec2 = ember::embVec<2, ember::embF32>;
using embVec3 = ember::embVec<3, ember::embF32>;
using embVec4 = ember::embVec<4, ember::embF32>;
EMB_NAMESPACE_END

EMB_DECLARE_VEC_TYPE_ALL(ember::embF32, F)
EMB_DECLARE_VEC_TYPE_ALL(ember::embF64, F64)
// EMB_DECLARE_VEC_TYPE_ALL(ember::embS32, S)
// EMB_DECLARE_VEC_TYPE_ALL(ember::embU32, U)
