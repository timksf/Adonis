#pragma once

#include "Adonis/Core.h"

namespace Adonis {

	namespace math {

		namespace constants {
			constexpr float PI_F = 3.1415927f;
			constexpr double PI_D = 3.14159265358979323846;
		}
		
		namespace literals {

			constexpr float operator""_degf(long double value) {
				return static_cast<float>(value) * constants::PI_F / 180.0f;
			}

			constexpr long double operator""_degd(long double value) {
				return value * constants::PI_D / 180.0;
			}

			constexpr float operator""_radf(long double value) {
				return static_cast<float>(value) * 180.0f / constants::PI_F;
			}

			constexpr long double operator""_radd(long double value) {
				return value * 180.0 / constants::PI_F;
			}

		}

	}

}

