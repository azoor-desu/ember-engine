#pragma once

#include <climits> // for CHAR_BIT
#include <cstring> // memset

#include "macros.h"
#include "types.h"

// TEMP
#include <bitset>

EMB_NAMESPACE_START

// template<embU32 size>
// struct Bitset
// {
//     using data_type = embU8;
//     static constexpr embU32 bits_per_chunk = sizeof(data_type) * CHAR_BIT;
//     static constexpr embU32 chunk_count = (size / bits_per_chunk) + (size % bits_per_chunk ? 1 : 0);
//     static constexpr embU32 struct_size_bytes = (size / CHAR_BIT) + (size % CHAR_BIT ? 1 : 0);

//     Bitset<size>()
//     {
//         std::memset(m_Data, 0, struct_size_bytes);
//     }

//   private:
//     data_type m_Data[chunk_count];
// };

// template<embU32 size>
// using embBitset = Bitset<size>;

template<embU32 size>
using embBitset = std::bitset<size>;

// 8-bit bitmask for bit operations
using embBitset8 = embBitset<8>;
// 16-bit bitmask for bit operations
using embBitset16 = embBitset<16>;
// 32-bit bitmask for bit operations
using embBitmask32 = embBitset<32>;
// 64-bit bitmask for bit operations
using embBitset64 = embBitset<64>;

EMB_NAMESPACE_END
