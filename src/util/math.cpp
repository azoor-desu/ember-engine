/// \file 
/// <pre>
/// <para>	<b>File:</b>	Math.cpp</para>
/// <para>	<b>Authors:</b>	Loo Jun Yi (100%)</para>
/// <para>	<b>Emails:</b>	l.junyi@digipen.edu</para>
/// <para>	<b>Date:</b>	27/09/2022</para>
/// </pre>
/// <summary>
/// Contains functions for advanced math operations
/// \n All content (c) 2022 DigiPen Institute of Technology Singapore. All rights reserved.
/// </summary>
#include <cmath>
#include <limits>
#include <type_traits>
#include <algorithm>

namespace LE {

	// References: https://en.wikipedia.org/wiki/Fast_inverse_square_root

	/// Returns an approximation of 1/sqrt(value).
	float FastInvSqrt(float number, int iterations) {
		// Enable only on IEEE 754, else stop program
		static_assert(std::numeric_limits<float>::is_iec559);

		// Evil magic constant. Don't try to understand.
		// Magic Number - float bit-shifted by 1 as an int = approximate 1/sqrt value.
		float y = 
			std::bit_cast<float>(0x5F375A86 - (std::bit_cast<std::uint32_t>(number) >> 1));

		// newtons method
		for (int i = 0; i < iterations; ++i) {
			y = y * (1.5f - (number * 0.5f * y * y));
		}

		return y;
	}

	/// Returns an approximation of 1/sqrt(value).
	double FastInvSqrt(double number, int iterations) {
		// Enable only on IEEE 754, else stop program
		static_assert(std::numeric_limits<float>::is_iec559);

		// Evil magic constant. Don't try to understand.
		// Magic Number - float bit-shifted by 1 as an int = approximate 1/sqrt value.
		double y =
			std::bit_cast<double>(0x5FE6EB50C7B537A9 - (std::bit_cast<std::uint64_t>(number) >> 1));

		// newtons method
		for (int i = 0; i < iterations; ++i) {
			y = y * (1.5 - (number * 0.5 * y * y));
		}

		return y;
	}
}

