#include "pch-engine.h"

#include "util/macros.h"
#include "util/types.h"

#include "resourcemanager.h"

EMB_NAMESPACE_START

std::array<std::array<ResourceTid, RESOURCEMANAGER_RESOURCE_COUNT>, (embU64)ResourceType::MAX_VAL> ResourceStoreLookup::m_Data = {};

EMB_NAMESPACE_END
