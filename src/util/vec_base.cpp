#include "defines.h"
#include "vec_base.h"

EMB_NAMESPACE_START

// To reduce template compilation overhead by not including the template defs in every TU.
// Still includes template headers though, increasing linking time. Can't avoid, non-templated classes will have same overhead.
// https://arne-mertz.de/2019/02/-template-reduce-compile-times/

template class embVec2<embF32>;
template class embVec2<embS32>;
template class embVec2<embU32>;

template class embVec3<embF32>;
template class embVec3<embS32>;
template class embVec3<embU32>;

template class embVec4<embF32>;
template class embVec4<embS32>;
template class embVec4<embU32>;

EMB_NAMESPACE_END
