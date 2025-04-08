#ifndef RNG_H
#define RNG_H

#include "Defines.h"
#include <vector>

namespace TE
{
constexpr TEuint32 START_SEED = 0;

/// <summary>
/// Set user specific seed.
/// </summary>
/// <param name="">Integer 32-bits of user specified seed.</param>
/// <return>Nothing.</return>
void SetSeed(TEuint32 seed);

/// <summary>
/// Returns a random number in the range of [start,end].
/// </summary>
/// <param name="start">From this number (inclusive).</param>
/// <param name="end">To this number (inclusive).</param>
/// <return>Integer 32-bits value between user specified start & end.</return>
TEint32 RandomRange(TEint32 start, TEint32 end);

/// <summary>
/// Gets random number from a range by user input [start,end].
/// </summary>
/// <param name="start">From this number (inclusive).</param>
/// <param name="end">To this number (inclusive).</param>
/// <return>Float value between user specified start & end.</return>
TEfloat RandomRange(TEfloat start, TEfloat end);

/// <summary>
/// Returns a random number in the range of [start,end].
/// </summary>
/// <param name="start">From this number (inclusive).</param>
/// <param name="end">To this number (inclusive).</param>
/// <return>Double value between user specified start & end.</return>
TEdouble RandomRange(TEdouble start, TEdouble end);

/// <summary>
/// Generates a vector of integers from 0 to (count - 1), and randomizes the order of
/// this vector of integers. E.g. If given 5, will generate 0,1,2,3,4 and randomize the order
/// to give something like 2,1,4,3,0
/// </summary>
/// <param name="count">The amount of numbers to generate.</param>
/// <return>Randomized order of the vector</return>
std::vector<TEint32> GetRandomOrder(TEint32 count);
} // namespace TE

#endif // !RNG_H
