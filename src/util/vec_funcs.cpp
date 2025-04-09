#include "vec_funcs.h"

EMB_NAMESPACE_START

#define EXTERN_MACRO(vecType, valueType) \
    template valueType VecDot<vecType<valueType>>(const vecType<valueType>& a, \
                                                  const vecType<valueType>& b); \
    template valueType VecMagnitudeSquare<vecType<valueType>>(const vecType<valueType>& a); \
    template valueType VecMagnitude<vecType<valueType>>(const vecType<valueType>& a); \
    template valueType VecDistanceSquare<vecType<valueType>>(const vecType<valueType>& a, \
                                                             const vecType<valueType>& b); \
    template valueType VecDistance<vecType<valueType>>(const vecType<valueType>& a, \
                                                       const vecType<valueType>& b); \
    template vecType<valueType> VecNormalize<vecType<valueType>>(const vecType<valueType>& a); \
    template vecType<valueType> VecNormalizeFast<vecType<valueType>>(const vecType<valueType>& a);

EXTERN_MACRO(embVec2, embF32)
EXTERN_MACRO(embVec3, embF32)
EXTERN_MACRO(embVec4, embF32)
#undef EXTERN_MACRO

EMB_NAMESPACE_END
