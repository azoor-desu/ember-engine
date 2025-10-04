#include "pch-engine.h"

#include "util/macros.h"
#include "util/macros_util.h"
#include "util/types.h"

#include "resourcemanager.h"

EMB_NAMESPACE_START

//-------------------------------------------------------------------//
//                            ResourceHandle                         //
//-------------------------------------------------------------------//

void* ResourceHandle::GetData() const noexcept
{
    // validation: Check if existing store matches GUID.
    EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
        m_Parity == ResourceManager::Instance().GetResourceStore().GetParityData((ResourceType)m_TypeIndex, m_SlotIndex),
        "Parity bit has changed, resource is not correct anymore."));

    return ResourceManager::Instance().GetResourceStore().GetResourceData((ResourceType)m_TypeIndex, m_SlotIndex);
}

//-------------------------------------------------------------------//
//                            ResourceManager                        //
//-------------------------------------------------------------------//

ResourceHandle ResourceManager::GetResourceHandle(embResourceTypeGuid typeGuid, embResourceGuid resGuid) noexcept
{
    ResourceType resType = EMB_X_ENUM_FROM_HASH(ResourceType, typeGuid);
    ResourceStore::ResourceSlotIndex slotIndex = m_ResourceStore.GetResourceDataSlotFromGuid(resType, resGuid);

    // if resource is not loaded, load it and use new slot.
    if (slotIndex == RESMGR_INVALID_SLOT)
    {
        embRawPointer newResource = LoadResource(typeGuid, resGuid);
        slotIndex = m_ResourceStore.AddNewResourceData(resType, resGuid, newResource);
    }

    return ResourceHandle(resType, slotIndex);
}

EMB_NAMESPACE_END
