#include "pch-engine.h"

#include "util/macros.h"
#include "util/macros_util.h"
#include "util/types.h"

#include "resourcemanager.h"

EMB_NAMESPACE_START

//-------------------------------------------------------------------//
//                            ResourceHandle                         //
//-------------------------------------------------------------------//

embMap<embU32, embU16> ResourceHandle::s_HandleRefCount {};

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

ResourceHandle::~ResourceHandle() // destructor
{
        // decrement ref counter
    embU32 key = (embU32)m_SlotIndex | ((embU32)m_TypeIndex << 16); // hardcode slot to u16
    EMB_ASSERT_HARD(s_HandleRefCount[key] > 0, "attempting to decrement ref count when count is already 0!");
    s_HandleRefCount[key]--;
    printf("destructor: ref count for key %u is %u\n", key, s_HandleRefCount[key]); // todo remove

    // If count == 0, unload resource.
    // TODO implement smarter logic to defer unloading after a little bit longer?
    if (s_HandleRefCount[key] == 0)
    {
        ResourceManager::Instance().UnloadResource((ResourceType)m_TypeIndex, m_SlotIndex);
    }
}

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
