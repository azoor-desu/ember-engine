#pragma once

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P)
#endif
#ifndef UNUSED_PARAM
#define UNUSED_PARAM(P) (P)
#endif
#ifndef UNUSED
#define UNUSED(P) (P)
#endif



#define EMB_NAMESPACE_START \
    namespace ember \
    {
#define EMB_NAMESPACE_END } // namespace ember

// Toggles

#define EMB_DEBUG_LEVEL 5
