#pragma once
#include "util/containers.h"
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
#    define EMB_IFDEF_VALIDATE_RESMGR(code) code
#else
#    define EMB_IFDEF_VALIDATE_RESMGR(code)
#endif

using embRawPointer = void*;
using embResourceGuid = embU32;

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

    ENUM_COUNT
};

//-------------------------------------------------------------------//
//                           ResourceStore                           //
//-------------------------------------------------------------------//

// Stores arrays of raw pointers.
// Does not manage resource lifetime!
class ResourceStore
{
  public:
    using ResourceSlotIndex = embU32;

    embRawPointer GetResourceData(ResourceType resType, ResourceSlotIndex slot) const noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESOURCEMANAGER_RESOURCE_COUNT,
                        "resource slot index out of range");

        // check that the corresponding m_PointerGuids is not 0
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            m_PointerGuids[(embSizeT)resType][slot] != 0,
            "Desynchronized m_PointerGuids and m_Pointers, possibly prior to call."));

        return m_Pointers[(embSizeT)resType][slot]; // fast
    }

    // Warning: Does not remove the allocated data. Only removes the related entires in this class.
    void RemoveResourceDataEntry(ResourceType resType, ResourceSlotIndex slot) noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESOURCEMANAGER_RESOURCE_COUNT,
                        "resource slot index out of range");

        // check for double free
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            m_PointerGuids[(embSizeT)resType][slot] != 0,
            "ResourceStore double free!"));

        m_PointerGuids[(embSizeT)resType][slot] = 0;
        m_Pointers[(embSizeT)resType][slot] = nullptr;
    }

    bool CheckResourceDataExistsFromGuid(ResourceType resType, embResourceGuid resGuid) const noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");

        for (embU32 i = 0; i < RESOURCEMANAGER_RESOURCE_COUNT; i++)
        {
            if (m_PointerGuids[(embSizeT)resType][i] == resGuid)
            {
                return true;
            }
        }
        return false;
    }

    // Adds new entry to the store.
    void SetNewResourceData(ResourceType resType, embResourceGuid resGuid, embRawPointer ptr) noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");

        // iF resource is already in store, error.
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            !CheckResourceDataExistsFromGuid(resType, resGuid),
            "attempted to set new resource while it is already in store"));

        // Find new slot and set.
        for (embU32 i = 0; i < RESOURCEMANAGER_RESOURCE_COUNT; i++)
        {
            if (m_PointerGuids[(embSizeT)resType][i] == 0)
            {
                EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
                    m_Pointers[(embSizeT)resType][i] == nullptr,
                    "Desynchronized m_PointerGuids and m_Pointers, possibly prior to call."));

                m_PointerGuids[(embSizeT)resType][i] = resGuid;
                m_Pointers[(embSizeT)resType][i] = ptr;
                break;
            }
        }

        // crash if no more slots
        EMB_ASSERT_HARD(false,
                        "unable to SetNewResourceData, ran out of slots! consider increasing RESOURCEMANAGER_RESOURCE_COUNT.");
    }

    using PointerArray = embFixedSizeArray<embRawPointer, RESOURCEMANAGER_RESOURCE_COUNT>;
    embFixedSizeArray<PointerArray, (embU64)ResourceType::ENUM_COUNT> m_Pointers;

    using GuidArray = embFixedSizeArray<embResourceGuid, RESOURCEMANAGER_RESOURCE_COUNT>;
    embFixedSizeArray<GuidArray, (embU64)ResourceType::ENUM_COUNT> m_PointerGuids;
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
        return (T*)ResourcePtrStore::GetEntryData(gResourceStores[m_TypeIndex], m_SlotIndex);
    }

    T& operator->() const noexcept { return *get(); }

  public:
    embU32 m_TypeIndex : RESOURCEMANAGER_TYPE_INDEX_BITS;
    embU32 m_SlotIndex : RESOURCEMANAGER_SLOT_INDEX_BITS;
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
