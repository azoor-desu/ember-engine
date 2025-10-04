#pragma once
#include "util/containers.h"
#include "util/hash.h"
#include "util/macros.h"
#include "util/macros_debug.h"
#include "util/macros_util.h"
#include "util/math.h"
#include "util/str.h"
#include "util/types.h"

#include <array>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <set>
#include <unistd.h>
#include <unordered_map>
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
using embResourceTypeGuid = embU32;

constexpr embU32 RESHDL_TYPE_INDEX_BITS = 6; // 64 possible resource types
constexpr embU32 RESHDL_SLOT_INDEX_BITS = 10; // 1024 possible resources of each type in data array at a time
constexpr embU32 RESHDL_PARITY_BITS = 32 - RESHDL_TYPE_INDEX_BITS - RESHDL_SLOT_INDEX_BITS; // Target 32 bit size for RESHDL
constexpr embU32 RESMGR_RESOURCE_COUNT = PowerIntUnsigned((embU32)2, RESHDL_SLOT_INDEX_BITS); // 1024, same as above
constexpr embU32 RESMGR_INVALID_SLOT = embU32_MAX;

EMB_ASSERT_STATIC(RESHDL_PARITY_BITS <= 64, "Parity takes up too much bits, check for underflow!");

//-------------------------------------------------------------------//
//                                 Enum                              //
//-------------------------------------------------------------------//

// Create enum, tostr and tohash using X-macro
#define X_LIST_RESOURCETYPE(X) \
    X(ResourceType, SHADER_VERTEX) \
    X(ResourceType, SHADER_FRAG) \
    X(ResourceType, SHADER_PROGRAM) \
    X(ResourceType, TEXTURE_SPRITE) \
    X(ResourceType, TEXTURE_ALBEDO) \
    X(ResourceType, AUDIO) \
    X(ResourceType, FONT_TTF) \
    X(ResourceType, SCENE)

EMB_X_DEF_ENUM(ResourceType, embU8, X_LIST_RESOURCETYPE)
EMB_ASSERT_STATIC((embU32)ResourceType::ENUM_COUNT <= PowerIntUnsigned((embU32)2, RESHDL_TYPE_INDEX_BITS),
                  "ResourceType count exceeds what RESHDL_TYPE_INDEX_BITS can support, consider increasing RESHDL_TYPE_INDEX_BITS");

EMB_X_DEF_ENUM_TO_STR(ResourceType, X_LIST_RESOURCETYPE)
EMB_X_DEF_ENUM_TO_HASH(ResourceType, X_LIST_RESOURCETYPE)
EMB_X_DEF_ENUM_FROM_HASH(ResourceType, X_LIST_RESOURCETYPE)

#undef X_LIST_RESOURCETYPE

//-------------------------------------------------------------------//
//                            ResourceHandle                         //
//-------------------------------------------------------------------//

// Handles are RUNTIME-ONLY references to raw pointers.
// Handles ALWAYS assume the data they point to is correct.
struct ResourceHandle
{
  private:
    ResourceHandle(ResourceType type, embU16 slot) // private default constructor, only "factory" can create
        : m_TypeIndex {(embU16)type}
        , m_SlotIndex {slot}
    {
        // increment ref counter
        embU32 key = (embU32)slot | ((embU32)type << 16); // hardcode slot to u16
        s_RefCount[key]++;
        printf("copyconstructor: ref count for key %u is %u\n", key, s_RefCount[key]);
    }

  public:
    ResourceHandle(const ResourceHandle& obj) // copy constructor
        : m_TypeIndex {(embU16)obj.m_TypeIndex}
        , m_SlotIndex {obj.m_SlotIndex}
    {
        // increment ref counter
        embU32 key = (embU32)m_SlotIndex | ((embU32)m_TypeIndex << 16); // hardcode slot to u16
        s_RefCount[key]++;
        printf("copyassign: ref count for key %u is %u\n", key, s_RefCount[key]);
    }

    ResourceHandle& operator=(const ResourceHandle& obj) // copy assignment
    {
        return *this = ResourceHandle(obj);
    }

    ResourceHandle(ResourceHandle&& obj) noexcept // move constructor
        : m_TypeIndex {(embU16)obj.m_TypeIndex}
        , m_SlotIndex {obj.m_SlotIndex}
    {}

    ResourceHandle& operator=(ResourceHandle&& obj) noexcept // move assignment
    {
        m_TypeIndex = obj.m_TypeIndex;
        m_SlotIndex = obj.m_SlotIndex;
        return *this;
    }

    ~ResourceHandle() // destructor
    {
        // decrement ref counter
        embU32 key = (embU32)m_SlotIndex | ((embU32)m_TypeIndex << 16); // hardcode slot to u16
        s_RefCount[key]--;
        printf("destructor: ref count for key %u is %u\n", key, s_RefCount[key]);
    }

    void* GetData() const noexcept;

  private:
    embU16 m_TypeIndex : RESHDL_TYPE_INDEX_BITS;
    embU16 m_SlotIndex : RESHDL_SLOT_INDEX_BITS;
    EMB_IFDEF_VALIDATE_RESMGR(embU16 m_Parity : RESHDL_PARITY_BITS);

    // global map just to count handle references
    static embMap<embU32, embU16> s_RefCount;

    friend class ResourceManager;
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

    embRawPointer GetResourceData(const ResourceType resType, const ResourceSlotIndex slot) const noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESMGR_RESOURCE_COUNT,
                        "ResourceSlotIndex out of range");

        // check that the corresponding m_PointerGuids is not 0
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            m_PointerGuids[(embSizeT)resType][slot] != 0,
            "Desynchronized m_PointerGuids and m_Pointers, possibly prior to call."));

        return m_Pointers[(embSizeT)resType][slot]; // fast
    }

    ResourceSlotIndex GetResourceDataSlotFromGuid(const ResourceType resType, const embResourceGuid resGuid) const noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");

        for (embU32 i = 0; i < RESMGR_RESOURCE_COUNT; i++)
        {
            if (m_PointerGuids[(embSizeT)resType][i] == resGuid)
            {
                return i;
            }
        }
        return RESMGR_INVALID_SLOT;
    }

    // Warning: Does not remove the allocated data. Only removes the related entires in this class.
    void RemoveResourceDataEntry(const ResourceType resType, const ResourceSlotIndex slot) noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESMGR_RESOURCE_COUNT,
                        "ResourceSlotIndex out of range");

        // check for double free
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            m_PointerGuids[(embSizeT)resType][slot] != 0,
            "ResourceStore double free!"));

        m_PointerGuids[(embSizeT)resType][slot] = 0;
        m_Pointers[(embSizeT)resType][slot] = nullptr;
    }

    // Adds new entry to the store.
    ResourceSlotIndex AddNewResourceData(const ResourceType resType, const embResourceGuid resGuid, const embRawPointer ptr) noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(ptr != nullptr,
                        "cannot set nullptr as resource!");

        // iF resource is already in store, error.
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            GetResourceDataSlotFromGuid(resType, resGuid) == RESMGR_INVALID_SLOT,
            "attempted to set new resource while it is already in store"));

        // Find new slot and set.
        for (embU32 i = 0; i < RESMGR_RESOURCE_COUNT; i++)
        {
            if (m_PointerGuids[(embSizeT)resType][i] == 0)
            {
                EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
                    m_Pointers[(embSizeT)resType][i] == nullptr,
                    "Desynchronized m_PointerGuids and m_Pointers, possibly prior to call."));

                m_PointerGuids[(embSizeT)resType][i] = resGuid;
                m_Pointers[(embSizeT)resType][i] = ptr;
                return i;
            }
        }
        // crash if no more slots
        EMB_ASSERT_HARD(false,
                        "unable to SetNewResourceData, ran out of slots! consider increasing RESOURCEMANAGER_RESOURCE_COUNT.");
        return RESMGR_INVALID_SLOT;
    }

    // Modifies existing data.
    // Do not allow modification of empty slots.
    void SetNewResourceData(const ResourceType resType, const ResourceSlotIndex slot, const embRawPointer ptr) noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESMGR_RESOURCE_COUNT,
                        "ResourceSlotIndex out of range");
        EMB_ASSERT_HARD(ptr != nullptr,
                        "cannot set nullptr as resource!");

        // iF target slot is empty, don't allow modification
        EMB_IFDEF_VALIDATE_RESMGR(EMB_ASSERT_HARD(
            m_PointerGuids[(embSizeT)resType][slot] != 0,
            "attempted to modify slot that has no data yet. Only allow modification of slots returned by AddNewResourceData"));

        m_Pointers[(embSizeT)resType][slot] = ptr;
    }

#ifdef EMB_DEF_VALIDATE_RESMGR
    embU16 GetParityData(const ResourceType resType, const ResourceSlotIndex slot) const noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESMGR_RESOURCE_COUNT,
                        "ResourceSlotIndex out of range");

        return m_Parity[(embSizeT)resType][slot];
    }

    void SetParityData(const ResourceType resType, const ResourceSlotIndex slot, const embU16 parityVal) noexcept
    {
        EMB_ASSERT_HARD(resType < ResourceType::ENUM_COUNT,
                        "resType out of range");
        EMB_ASSERT_HARD(slot < RESMGR_RESOURCE_COUNT,
                        "ResourceSlotIndex out of range");

        m_Parity[(embSizeT)resType][slot] = parityVal;
    }
#endif

    using PointerArray = embFixedSizeArray<embRawPointer, RESMGR_RESOURCE_COUNT>;
    embFixedSizeArray<PointerArray, (embU64)ResourceType::ENUM_COUNT> m_Pointers {};

    using GuidArray = embFixedSizeArray<embResourceGuid, RESMGR_RESOURCE_COUNT>;
    embFixedSizeArray<GuidArray, (embU64)ResourceType::ENUM_COUNT> m_PointerGuids {};

#ifdef EMB_DEF_VALIDATE_RESMGR
    using ParityArray = embFixedSizeArray<embU16, RESMGR_RESOURCE_COUNT>;
    embFixedSizeArray<ParityArray, (embU64)ResourceType::ENUM_COUNT> m_Parity {};
#endif
};

//-------------------------------------------------------------------//
//                            ResourceManager                        //
//-------------------------------------------------------------------//

// Manager that handles storing and lookup of resources
class ResourceManager
{
  public:
    EMB_CLASS_SINGLETON_MACRO(ResourceManager)

    ResourceStore& GetResourceStore() noexcept
    {
        return m_ResourceStore;
    }

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

    ResourceHandle GetResourceHandle(embResourceTypeGuid typeGuid, embResourceGuid resGuid) noexcept;

  private:
    // Loads raw data according to different implementations.
    // Resource lifetime is NOT managed by this function. Caller will manage.
    void* LoadResource(embResourceTypeGuid typeGuid, embResourceGuid resGuid)
    {
        // TODO grabs the loaded metadata, load data into game memory from asset files
        return (void*)1234; // temp testing, return something other than nullptr
    }

  private:
    ResourceStore m_ResourceStore;
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
