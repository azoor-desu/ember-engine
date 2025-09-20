#include "pch-engine.h"

#include "util/macros.h"
#include "util/types.h"

#include "resourcemanager.h"

EMB_NAMESPACE_START

std::array<bool, RESOURCEMANAGER_RESOURCE_COUNT> ResourceStoreLookup::isUsed = {};

EMB_NAMESPACE_END
