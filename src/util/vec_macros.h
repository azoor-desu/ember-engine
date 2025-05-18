#pragma once

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
