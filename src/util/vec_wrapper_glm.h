#pragma once

#include "types.h"

// Feature toggles (before #include "glm/glm.hpp") TODO: MOVE TO SOME COMMON PLACE FOR MATRIX TOO
//#define GLM_FORCE_MESSAGES // TODO: Toggle on/off with debug
#define GLM_FORCE_EXPLICIT_CTOR
//#define GLM_FORCE_INLINE // TODO: Only enable for release
//#define GLM_FORCE_INTRINSICS // TODO: Only enable for release

#include "glm/glm.hpp"
#include <glm/ext/vector_float2.hpp>

EMB_NAMESPACE_START

// based on glm vec class
template<embU32 Size, typename T>
using embVec = glm::vec<Size, T, glm::packed_highp>;

EMB_NAMESPACE_END

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/

#define EMB_DEFINE_VEC_TYPE_INTERNAL(size, valueType, valueShorthand) \
    template struct glm::vec<size, valueType, glm::packed_highp>;

#define EMB_DEFINE_VEC_TYPE(size, valueType, valueShorthand) \
    namespace glm \
    { \
    EMB_DEFINE_VEC_TYPE_INTERNAL(size, valueType, valueShorthand) \
    }

#define EMB_DEFINE_VEC_TYPE_ALL(valueType, valueShorthand) \
    EMB_DEFINE_VEC_TYPE(2, valueType, valueShorthand) \
    EMB_DEFINE_VEC_TYPE(3, valueType, valueShorthand) \
    EMB_DEFINE_VEC_TYPE(4, valueType, valueShorthand)

#define EMB_DECLARE_VEC_TYPE(size, valueType, valueShorthand) \
    EMB_NAMESPACE_START \
    using embVec##size##valueShorthand = embVec<size, valueType>; \
    EMB_NAMESPACE_END \
    namespace glm \
    { \
    extern EMB_DEFINE_VEC_TYPE_INTERNAL(size, valueType, valueShorthand) \
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
EMB_DECLARE_VEC_TYPE_ALL(ember::embS32, S)
EMB_DECLARE_VEC_TYPE_ALL(ember::embU32, U)
