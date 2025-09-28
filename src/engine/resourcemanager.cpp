#include "pch-engine.h"

#include "util/macros.h"
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
    ResourceHandle ret;

    // determine what ResourceType this guy is.
    // damn i think i fucked myself with using enums as ResourceType instead of hashes...
    // I need to map the internal enum with external ResourceGuid
    // I need some translation table that can do this both ways.
    // Due to Enum having to change frequently, translation table needs to be generated on the fly.
    // I could just hash the string of the enum cos that's the only consistent thing.
    // Build a feature for the translation table so that i can manually inject custom mappings in case of "migrations"
    // e.g. changing name from AUDIO to AUDIO_GENERAL. I want to map all of the old AUDIO hashes into AUDIO_GENERAL.



    ResourceStore::ResourceSlotIndex slotIndex = m_ResourceStore.GetResourceDataSlotFromGuid(const ResourceType resType, resGuid);

    return ret;
}

EMB_NAMESPACE_END
