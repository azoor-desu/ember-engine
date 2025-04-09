#include "vec_utils.h"
// To reduce template compilation overhead by not including the template defs in every TU.
// Still includes template headers though, increasing linking time. Can't avoid, non-templated classes will have same overhead.
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/

#define EXTERN_MACRO(valueType) \
    template class std::formatter<ember::embVec2<ember::valueType>>; \
    template class std::formatter<ember::embVec3<ember::valueType>>; \
    template class std::formatter<ember::embVec4<ember::valueType>>;

EXTERN_MACRO(embF32)
// EXTERN_MACRO(embF64)
// EXTERN_MACRO(embU8)
// EXTERN_MACRO(embS32)
// EXTERN_MACRO(embU32)

#undef EXTERN_MACRO
