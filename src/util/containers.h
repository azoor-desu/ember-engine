#pragma once

#include "types.h"
#include <array>
#include <unordered_map>
#include <vector>

// to split this up... too many containers!

EMB_NAMESPACE_START

template<typename T>
using embArray = std::vector<T>; // use u16 by default, 65k elements.

template<typename T>
using embLargeArray = std::vector<T>; // use u32 or sth instead of u16

template<typename T>
using embInplaceArray = std::vector<T>; // slap on stack default, excess goes to heap.

template<typename T, embU64 size>
using embFixedSizeArray = std::array<T, size>; // fixed size array

template<typename T>
using embPingPongBuffer = std::vector<T>;

template<typename T>
using embQueueBuffer = std::vector<T>;

template<typename T>
using embCircularBuffer = std::vector<T>;

template <typename Key, typename Value>
using embMap = std::unordered_map<Key, Value>;

template<typename T>
using embList = std::vector<T>;

template<typename T>
using embSet = std::vector<T>;

template<typename T>
using embStreeeeng = std::vector<T>;

template<typename T>
using embBitset = std::vector<T>;

EMB_NAMESPACE_END
