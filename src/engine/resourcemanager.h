#pragma once
#include "util/hash.h"
#include "util/macros.h"
#include "util/math.h"
#include "util/str.h"
#include "util/types.h"

#include <array>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

EMB_NAMESPACE_START

using embResourceGUID = embGenericGUID;
using embResourceTypeID = embU8;

constexpr embU32 RESOURCEMANAGER_TYPE_INDEX_BITS = 6; // 64 possible resource types
constexpr embU32 RESOURCEMANAGER_TYPE_COUNT = PowerIntUnsigned((embU32)2, RESOURCEMANAGER_TYPE_INDEX_BITS);
constexpr embU32 RESOURCEMANAGER_SLOT_INDEX_BITS = 10; // 1024 possible resources of each type in data array at a time
constexpr embU32 RESOURCEMANAGER_SLOT_COUNT = PowerIntUnsigned((embU32)2, RESOURCEMANAGER_SLOT_INDEX_BITS);

//-------------------------------------------------------------------//
//                            ResourceHandle                         //
//-------------------------------------------------------------------//

// Handle to get resources
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
    // Else, call load and block.
        return nullptr; // TODO
    }

    T& operator->() const noexcept { return *get(); }

  public:
    constexpr static embU32 c_StructSizeBits = 32;

    embU32 m_TypeIndex : RESOURCEMANAGER_TYPE_INDEX_BITS; // the type this resource belongs to. Corresponds to ResourceManager::m_ResourceStoreHashes
    embU32 m_SlotIndex : RESOURCEMANAGER_SLOT_INDEX_BITS; // the slot to find this resource at in its array. Corresponds to ResourceStore::m_Data
    embU32 m_Parity : c_StructSizeBits - RESOURCEMANAGER_TYPE_INDEX_BITS - RESOURCEMANAGER_SLOT_INDEX_BITS; // for checking if slot is still valid (data is deallocated and replaced?)
};

//-------------------------------------------------------------------//
//                     IResourceStore/ResourceStore                  //
//-------------------------------------------------------------------//

// Common Interface for resource stores
class IResourceStore
{
  public:
    virtual ~IResourceStore() {} // to shut up unique_ptr complaining about having a deleted destructor
    virtual int GetSomeShit();
    virtual embHash64 GetID();
};

// holds an array of a specific resource type
template <typename T>
class ResourceStore : IResourceStore
{
  public:
    void AddToData(embResourceGUID resourceGUID)
    {
        
    }
    void RemoveFromData(embResourceGUID resourceGUID)
    {
    }

  private:
    std::vector<T> m_Data;
    std::vector<embResourceGUID> m_DataGUID;
};

//-------------------------------------------------------------------//
//                            ResourceManager                        //
//-------------------------------------------------------------------//

// Manager that handles storing and lookup of resources
class ResourceManager
{
  private:
  public:
    enum class ResourceType : embU8
    {
        SHADER_VERTEX,
        SHADER_FRAG,
        SHADER_COMPUTE,
        SHADER_PROGRAM,

        TEXTURE_SPRITE,
        TEXTURE_ALBEDO,

        AUDIO,
        MODEL_INTERNAL,
        MODEL_FBX, // or mesh...?
        FONT_TTF,
        SCENE,
        OPENGL_VAO, // ??? Maybe model will do...?
    };

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
        m_ResourceStores.push_back(std::unique_ptr<IResourceStore>((IResourceStore*)new ResourceStore<T>));
    }

  public:
  private:
    std::vector<embHash64> m_ResourceStoreHashes; // hashes here correspond to each elem of m_ResourceStores. Used for lookup.
    std::vector<std::unique_ptr<IResourceStore>> m_ResourceStores;

    // each resource store container is all over the place...
    // no way to have each resource store easily be in contiguous memory next to each other.
    // theoretically, it's possible, as all ResourceStore<T>s have the same size. (one std::vector...)
    // Can somehow magically put a bunch of these stores contiguously, and magically force a loop over these items.
    // super sketchy design + compiler may be completely unable to optimize it.
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



// TEMP TESTING AND SHIT
class ExampleResource1 : IResourceStore
{
  public:
    void* m_Data;
    int someshit;
    bool othershit;

    int GetSomeShit() override final
    {
        return someshit;
    }

    embHash64 GetID() override final
    {
        return Hash::GetTypeHash<ExampleResource1>();
    }
};

EMB_NAMESPACE_END


// Future improvements:
// - Move the registration of types into compile time. This will significantly speed up lookups of types and shit.
//      > Caveat: Will require all resource types to be known to ResourceManager at compile time. A class declaration will do.
//      Most likely, more boilerplate code like allocating custom ResourceStore<T> types or generating a lookup table will require these custom types
//      to be inserted into all of these random locations throughout the code.
//      Is a pain in the ass to add/remove these types in multiple locations.
//      A custom parser and table generator can be a solution for this.
