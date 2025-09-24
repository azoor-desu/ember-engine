#pragma once
#include "util/hash.h"
#include "util/macros.h"
#include "util/math.h"
#include "util/str.h"
#include "util/types.h"

#include "debug.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <set>
#include <utility>
#include <vector>

EMB_NAMESPACE_START

// debug macros
#ifdef EMB_DEF_DEBUG
#    define EMB_DEF_VALIDATE_RESMGR
#endif
#ifdef EMB_DEF_VALIDATE_RESMGR
#    define EMB_IFDEF_VALIDATE_RESMGR (code) code
#else
#    define EMB_IFDEF_VALIDATE_RESMGR (code)
#endif

//using embResourceTid = embGenericGuid; // change to sth custom
//using embResourceTypeID = embU8;
using embRawPointer = void*;

constexpr embU32 RESOURCEMANAGER_HANDLE_SIZE_BITS = 32;
constexpr embU32 RESOURCEMANAGER_TYPE_INDEX_BITS = 6; // 64 possible resource types
constexpr embU32 RESOURCEMANAGER_TYPE_COUNT = PowerIntUnsigned((embU32)2, RESOURCEMANAGER_TYPE_INDEX_BITS);

constexpr embU32 RESOURCEMANAGER_SLOT_INDEX_BITS = 10; // 1024 possible resources of each type in data array at a time
constexpr embU32 RESOURCEMANAGER_SLOT_COUNT = PowerIntUnsigned((embU32)2, RESOURCEMANAGER_SLOT_INDEX_BITS);
constexpr embU32 RESOURCEMANAGER_PARITY_BITS = RESOURCEMANAGER_HANDLE_SIZE_BITS - RESOURCEMANAGER_TYPE_INDEX_BITS - RESOURCEMANAGER_SLOT_INDEX_BITS;

constexpr embU32 RESOURCEMANAGER_RESOURCE_COUNT = 2048;
constexpr embU32 INVALID_SLOT = embU32_MAX;

enum class ResourceType : embU8
{
    SHADER_VERTEX = 0,
    SHADER_FRAG,
    // SHADER_COMPUTE,
    SHADER_PROGRAM,

    TEXTURE_SPRITE,
    TEXTURE_ALBEDO,

    AUDIO,
    // MODEL_RAW,
    // MODEL_FBX, // or mesh...?
    FONT_TTF,
    SCENE, // stores collections of gameobjects
    // OPENGL_VAO, // ??? Maybe model will do...?

    MAX_VAL
};

struct ResourceTid
{
    ResourceType m_ResType : 8 {};
    embU32 m_ResPartialGuidP : 24 {}; // least significant portion of full resourceguid
};

//-------------------------------------------------------------------//
//                           ResourceStore                           //
//-------------------------------------------------------------------//

class ResourceStoreLookup
{
    ResourceStoreLookup() = delete; // static class

  public:
    static ResourceTid GetValue(ResourceType resType, embU32 index)
    {
#ifdef EMB_DEF_VALIDATE_RESMGR
        EMB_ASSERT_HARD(resType >= ResourceType::MAX_VAL || index >= RESOURCEMANAGER_RESOURCE_COUNT, "resType or resource index out of range");
#endif
        return m_Data[(embU64)resType][index];
    }

    static embU32 FindNewSlot(ResourceType resType, ResourceTid resourceGUID)
    {
#ifdef EMB_DEF_VALIDATE_RESMGR
        EMB_ASSERT_HARD(resType >= ResourceType::MAX_VAL, "resType out of range");
#endif
        // Find empty slot
        for (embU32 i = 0; i < RESOURCEMANAGER_RESOURCE_COUNT; i++)
        {
            if (m_Data[(embU64)resType][i] == 0)
            {
                return i;
            }
        }
        return INVALID_SLOT;
    }

    static void SetValue(ResourceType resType, embU32 index, embResourceTid resourceGUID)
    {
#ifdef EMB_DEF_VALIDATE_RESMGR
        EMB_ASSERT_HARD(resType >= ResourceType::MAX_VAL || index >= RESOURCEMANAGER_RESOURCE_COUNT, "resType or resource index out of range");
#endif
        m_Data[(embU64)resType][index] = resourceGUID;
    }

    static embU32 GetExistingSlotFromValue(ResourceType resType, embResourceTid resourceGUID)
    {
#ifdef EMB_DEF_VALIDATE_RESMGR
        EMB_ASSERT_HARD(resType >= ResourceType::MAX_VAL, "resType out of range");
#endif
        // Find resourceGUID
        for (embU32 i = 0; i < RESOURCEMANAGER_RESOURCE_COUNT; i++)
        {
            if (m_Data[(embU64)resType][i] == resourceGUID)
            {
                return i;
            }
        }
        return INVALID_SLOT;
    }

  private:
    static std::array<std::array<embResourceTid, RESOURCEMANAGER_RESOURCE_COUNT>, (embU64)ResourceType::MAX_VAL> m_Data;
};

// holds an array of a specific resource type
// store itself only provides a raw pointer according to handle data.
// Type casting is done by the handle.
class ResourceStore
{
  public:
    constexpr ResourceStore(ResourceType resType)
        : m_ResourceType {resType}
    {
    }

    ResourceType GetResourceType() const noexcept
    {
        return m_ResourceType;
    }

    static embU32 GetNewEntryIndex(ResourceStore resStore, embResourceTid resId)
    {
#ifdef EMB_DEF_VALIDATE_RESMGR
        // check if there is already an existing entry.
        // If there is, throw error. Not supposed to call this if there is an existing entry!
        if (ResourceStoreLookup::GetExistingSlotFromValue(resStore.m_ResourceType, resId) != INVALID_SLOT)
            Breakpoint();
#endif
        // Check which slots are free, return if yes.
        return ResourceStoreLookup::FindNewSlot(resStore.m_ResourceType, resId);
    }

    static embRawPointer GetEntryData(ResourceStore resStore, embU32 index)
    {
        return resStore.m_Data[index]; // fast
    }

    static void ReleaseEntry(ResourceStore resStore, embU32 index)
    {
        ResourceStoreLookup::SetValue(resStore.m_ResourceType, index, 0);
    }

  private:
    const embRawPointer m_Data[RESOURCEMANAGER_RESOURCE_COUNT] = {};
    const ResourceType m_ResourceType;

    // User wants to get data. handle.get()
    // Go to specific container (ResourceStore) according to m_TypeIndex.
    //  > Static cast from IResourceStore to  specific resource store. Should be optimized away.
    //  > Ends up as a direct call to funcs in ResourceStore
    // Go to specific slot in m_Data/m_DataGUID according to m_SlotIndex.
    // Checks if value is valid via parity bits or whatever. [check can be macro'd away... but probably a bad idea]
    // Returns data in m_Data slot
    // Pretty fast!
    // Caveats: entries CANNOT move. This forces fragmentation.
    // Workaround: have sparse/dense arrays, but introduces another layer of lookup.
};

constexpr ResourceStore gResourceStores[(embU64)ResourceType::MAX_VAL] = {
    ResourceStore(ResourceType::SHADER_VERTEX),
    ResourceStore(ResourceType::SHADER_FRAG),
    ResourceStore(ResourceType::SHADER_PROGRAM),
    ResourceStore(ResourceType::TEXTURE_SPRITE),
    ResourceStore(ResourceType::TEXTURE_ALBEDO),
    ResourceStore(ResourceType::AUDIO),
    ResourceStore(ResourceType::FONT_TTF),
    ResourceStore(ResourceType::SCENE),
};

//-------------------------------------------------------------------//
//                            ResourceHandle                         //
//-------------------------------------------------------------------//

// Handles are RUNTIME-ONLY references to raw pointers.
// Handles ALWAYS assume the data they point to is correct.
// Validation handling only happens during Handle Creation (From ResourceManager), and assuming that
// ResourceStore's create and delete are working properly according to ResourceHandle's REF COUNTING.
template <typename T>
struct ResourceHandle
{
    ResourceHandle(); // ADD REF COUNTER
    ~ResourceHandle(); // decrement ref counter

    T* get() const noexcept
    {
#ifdef EMB_DEF_VALIDATE_RESMGR
        // check if the current slot is the same as this resource handle's resourceId
        // If not, means data has been unloaded or not loaded. error out.
        if (ResourceStoreLookup::GetValue(m_TypeIndex, m_SlotIndex) !=)
            Breakpoint();
#endif
        return (T*)ResourceStore::GetEntryData(gResourceStores[m_TypeIndex], m_SlotIndex);
    }

    T& operator->() const noexcept { return *get(); }

  public:
    embU32 m_TypeIndex : RESOURCEMANAGER_TYPE_INDEX_BITS; // the type this resource belongs to. Corresponds to ResourceManager::m_ResourceStoreHashes
    embU32 m_SlotIndex : RESOURCEMANAGER_SLOT_INDEX_BITS; // the slot to find this resource at in its array. Corresponds to ResourceStore::m_Data
    embU32 m_Parity : RESOURCEMANAGER_PARITY_BITS; // for checking if slot is still valid (data is deallocated and replaced?)
};

//-------------------------------------------------------------------//
//                            ResourceManager                        //
//-------------------------------------------------------------------//

// Manager that handles storing and lookup of resources
class ResourceManager
{
  private:
  public:
    // Prior to loading Metadata, assets need to be packed/managed first.
    // at least, the generation of metadata needs to be done and saved somewhere.

    void LoadMetadata()
    {
        // loads in all metadata: Either from metadata files, or header in bin pack
        // Each resource has these data:
        // - Path to resource/which bin pack and what offset
        // - resource GUID
        // - resource type hash
    }

    // for quick and dirty loading. GUIDs should be preferred.
    // used when no Editor to handle GUIDs is available yet.
    embResourceTid NameToGUID(embHash64 pathHash)
    {
        return 0;
    }

    template <typename T>
    ResourceHandle<T> GetResourceHandle(embResourceTid resourceGUID)
    {
        // check if GUID exists or not. If not exist, error.
        // Else,
        // Create handle and return.
        ResourceHandle<T> ret;

        return ret;
    }

  public:
  private:
    std::vector<embHash64> m_ResourceStoreHashes; // each hash here corresponds to each elem of m_ResourceStores. Used for lookup.
    //std::vector<std::unique_ptr<IResourceStore>> m_ResourceStores;
};

// Type ID shit lookup
// store an array of typeids derived from locally hashing type names
// array should be populated in ORDER...
// embResourceTypeID relies on this order. Corresponds to the index.
// How to populate? and can it be done at compile time?
// Can the lookup function be a consteval function?
// Can the registration be macro'd and called from anywhere?
// Can other systems like component types use this same lookup system?
// this is basically RTTR lmao

EMB_NAMESPACE_END

// Current storage method:
// Declare & define a bunch of ResourceStore<TYPE> -> PUT into an array IResourceStore[20]
// To get data: handle.get() -> Look up IResourceStore[20] ->
//              get ResourceStore<TYPE> -> get data array -> find elem in data array & RETURN.


// Creating Handles/Loading resources
// Start out blank state. ResourceStore arrays all set up but empty.
// When a GameObject is deserialized, a ResourceId (to uniquely identify hehe.png) is provided
//  > Same thing when adding a new ResourceId to an object too, ResourceId will be provided
//  > ResourceId needs to contain the type too.
// Throw ResourceId into ResourceManager and ask for a ResourceHandle.
//  > Resourcemanager loads in the raw data, gets the pointer, and stores the raw pointer in ResourceStore.
//  > If data alr loaded, just return a handle.
// When all ResourceHandles disappear (ref counting == 0), remove that resource in ResourceStore.

// On deserialize:
// Need to be able to convert Handles back into ResourceId.
// When storing the raw pointer in ResourceStore, add the ResourceId as well in the backing store.
