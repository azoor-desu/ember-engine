#include "RNG.h"
#include <random>

namespace TE
{

std::random_device rd;
std::mt19937 seed_gen(START_SEED);

/// <summary>
/// Set seed of the random number generator engine.
/// </summary>
/// <param name="seed">Integer seed that user specify.</param>
/// <return>Nothing.</return>
void SetSeed(TEuint32 seed)
{
    // only change seed using this func.
    // if other funcs can change seed also, there may be a situation where
    // we need to test using a specific seed but the seed changes halfway thru execution
    // because somebody changed the seed using another func somewhere.
    // results in unexpected behaviour when we expect the seed to not change
    seed_gen.seed(seed);
}

/// <summary>
/// Returns a random number in the range of [start,end].
/// </summary>
/// <param name="start">From this number (inclusive).</param>
/// <param name="end">To this number (inclusive).</param>
/// <return>Integer 32-bits value between user specified start & end.</return>
TEint32 RandomRange(TEint32 start, TEint32 end)
{
    std::uniform_int_distribution<> distr(start, (TEint32)std::nextafter(end, INT32_MAX));
    return distr(seed_gen);
}

/// <summary>
/// Gets random number from a range by user input [start,end].
/// </summary>
/// <param name="start">From this number (inclusive).</param>
/// <param name="end">To this number (inclusive).</param>
/// <return>Float value between user specified start & end.</return>
TEfloat RandomRange(TEfloat start, TEfloat end)
{
    std::uniform_real_distribution<> distr(start, std::nextafter(end, FLT_MAX));
    return (TEfloat)distr(seed_gen);
}

/// <summary>
/// Returns a random number in the range of [start,end].
/// </summary>
/// <param name="start">From this number (inclusive).</param>
/// <param name="end">To this number (inclusive).</param>
/// <return>Double value between user specified start & end.</return>
TEdouble RandomRange(TEdouble start, TEdouble end)
{
    std::uniform_real_distribution<> distr(start, std::nextafter(end, DBL_MAX));
    return distr(seed_gen);
}

/// <summary>
/// Generates a vector of integers from 0 to (count - 1), and randomizes the order of
/// this vector of integers. E.g. If given 5, will generat 0,1,2,3,4 and randomize the order
/// to give something like 2,1,4,3,0
/// </summary>
/// <param name="count">The amount of numbers to generate.</param>
/// <return>Randomized order of the vector</return>
std::vector<TEint32> GetRandomOrder(TEint32 count)
{
    std::vector<TEint32> toReturn;
    toReturn.resize(count);
    for (TEint32 i{}; i < count; ++i)
    {
        toReturn.emplace_back(i);
    }
    std::shuffle(std::begin(toReturn), std::end(toReturn), seed_gen);
    return toReturn;
}
} // namespace TE
