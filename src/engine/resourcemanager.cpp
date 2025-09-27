#include "pch-engine.h"

#include "util/macros.h"
#include "util/types.h"

#include "resourcemanager.h"

EMB_NAMESPACE_START

std::array<std::array<ResourceTid, RESOURCEMANAGER_RESOURCE_COUNT>, (embU64)ResourceType::ENUM_COUNT> ResourcePointerListData::m_Data = {};

EMB_NAMESPACE_END
