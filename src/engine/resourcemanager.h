#pragma once
#include "util/hash.h"
#include "util/macros.h"
#include "util/math.h"
#include "util/str.h"
#include "util/types.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <set>
#include <utility>
#include <vector>

EMB_NAMESPACE_START

using embResourceGUID = embGenericGUID;
using embResourceTypeID = embU8;
using embRawPointer = void*;

constexpr embU32 RESOURCEMANAGER_HANDLE_SIZE_BITS = 32;
constexpr embU32 RESOURCEMANAGER_TYPE_INDEX_BITS = 6; // 64 possible resource types
constexpr embU32 RESOURCEMANAGER_TYPE_COUNT = PowerIntUnsigned((embU32)2, RESOURCEMANAGER_TYPE_INDEX_BITS);

constexpr embU32 RESOURCEMANAGER_SLOT_INDEX_BITS = 10; // 1024 possible resources of each type in data array at a time
constexpr embU32 RESOURCEMANAGER_SLOT_COUNT = PowerIntUnsigned((embU32)2, RESOURCEMANAGER_SLOT_INDEX_BITS);
constexpr embU32 RESOURCEMANAGER_PARITY_BITS = RESOURCEMANAGER_HANDLE_SIZE_BITS - RESOURCEMANAGER_TYPE_INDEX_BITS - RESOURCEMANAGER_SLOT_INDEX_BITS;

constexpr embU32 RESOURCEMANAGER_RESOURCE_COUNT = 4096;

enum class ResourceType : embU8
{
    SHADER_VERTEX = 0,
    SHADER_FRAG,
    SHADER_COMPUTE,
    SHADER_PROGRAM,

    TEXTURE_SPRITE,
    TEXTURE_ALBEDO,

    AUDIO,
    MODEL_RAW,
    MODEL_FBX, // or mesh...?
    FONT_TTF,
    SCENE, // stores collections of gameobjects
    OPENGL_VAO, // ??? Maybe model will do...?

    MAX_VAL
};

//-------------------------------------------------------------------//
//                           ResourceStore                           //
//-------------------------------------------------------------------//

class ResourceStoreLookup
{
  public:
    static std::array<bool, RESOURCEMANAGER_RESOURCE_COUNT> isUsed;
};

// holds an array of a specific resource type
// store itself only provides a raw pointer according to handle data.
// Type casting is done by the handle.
class ResourceStore
{
    static constexpr embU32 INVALID_SLOT = embU32_MAX;

  public:
    constexpr ResourceStore(ResourceType resType)
        : m_ResourceType {resType}
    {
    }

    ResourceType GetResourceType()
    {
        return m_ResourceType;
    }

    static embU32 GetNewEntryIndex(ResourceStore resStore)
    {
        // Check which slots are free, return if yes.
        for (int i = 0; i < ResourceStoreLookup::isUsed.size(); i++)
        {
            if (!ResourceStoreLookup::isUsed[i])
            {
                ResourceStoreLookup::isUsed[i] = true;
                return i;
            }
        }
        return INVALID_SLOT;
    }

    static embRawPointer GetEntryData(ResourceStore resStore, embU32 index)
    {
        // TODO: Debug ONLY. check against ResourceStoreLookup to see if resource is valid.
        return resStore.m_Data[index];
    }

    static void ReleaseEntry(ResourceStore resStore, embU32 index)
    {
        ResourceStoreLookup::isUsed[index] = false;
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



//-------------------------------------------------------------------//
//                            ResourceHandle                         //
//-------------------------------------------------------------------//

// Handles are RUNTIME-ONLY references to resource pointers.
// To serialize handles, convert them into GUIDs. have a dedicated thing to help convert.
// Resources MUST be loaded into the ResourceStore before a valid handle can be created.
template <typename T>
struct ResourceHandle
{
    ResourceHandle(); // ADD REF COUNTER
    ~ResourceHandle(); // decrement ref counter

    embBool CheckResourceDataLoaded() const noexcept
    {
        return false; // TODO
    }

    void LoadResourceData() const noexcept
    {
        // verify type is valid [Debug]
        // lookup slot with m_ResourceID
        // if exists, return.
        // Else, load... async if possible.
        // Slot = T::LoadIntoObject(m_ResourceID);
    }

    T* get() const noexcept
    {
        // verify type is valid [Debug]
        // lookup slot with m_ResourceID
        // if exists, return.
        // Else, call load and block until loaded

        //return ResourceStore<T>.m_DenseData[m_SlotIndex];

        return nullptr; // TODO
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
    embResourceGUID NameToGUID(embHash64 pathHash)
    {
        return 0;
    }

    template <typename T>
    ResourceHandle<T> GetResourceHandle(embResourceGUID resourceGUID)
    {
        // check if GUID exists or not. If not exist, error.
        // Else,
        // Create handle and return.
        ResourceHandle<T> ret;

        return ret;
    }

    template <typename T>
    void RegisterType()
    {
        constexpr embHash64 typeHash = Hash::GetTypeHash<T>();

        // check if alr exist
        for (auto hash : m_ResourceStoreHashes)
        {
            if (hash == typeHash)
                return;
        }

        m_ResourceStoreHashes.push_back(typeHash);
        //m_ResourceStores.push_back(std::unique_ptr<IResourceStore>((IResourceStore*)new ResourceStore<T>));
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


// Future improvements:
// - Move the registration of types into compile time. This will significantly speed up lookups of types and shit.
//      > Caveat: Will require all resource types to be known to ResourceManager at compile time. A class declaration will do.
//      Most likely, more boilerplate code like allocating custom ResourceStore<T> types or generating a lookup table will require these custom types
//      to be inserted into all of these random locations throughout the code.
//      Is a pain in the ass to add/remove these types in multiple locations.
//      A custom parser and table generator can be a solution for this.

// Current storage method:
// Declare & define a bunch of ResourceStore<TYPE> -> PUT into an array IResourceStore[20]
// To get data: handle.get() -> Look up IResourceStore[20] ->
//              get ResourceStore<TYPE> -> get data array -> find elem in data array & RETURN.



EMB_NAMESPACE_START

// idk register resources via static classes

constexpr ResourceStore resourceStores[3] = {
    ResourceStore(ResourceType::AUDIO),
    ResourceStore(ResourceType::MODEL_RAW),
    ResourceStore(ResourceType::MODEL_FBX),
};

EMB_NAMESPACE_END
