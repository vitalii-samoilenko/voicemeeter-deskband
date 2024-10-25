#pragma once

#include <limits>
#include <type_traits>

#include "type_traits.h"

namespace linear_algebra {
	template<typename TScalar>
	struct vector final {
		static_assert(
			::std::is_arithmetic_v<TScalar>,
			"TScalar must be of arithmetic type");

		TScalar x;
		TScalar y;

		constexpr vector() = default;
		vector(const vector&) = default;
		vector(vector&&) = default;

		~vector() = default;

		vector& operator=(const vector&) = default;
		vector& operator=(vector&&) = default;

		static constexpr vector origin() {
			return {
				::std::numeric_limits<TScalar>::denorm_min(),
				::std::numeric_limits<TScalar>::denorm_min()
			};
		};

		static constexpr vector infinity() {
			return {
				::std::numeric_limits<TScalar>::max(),
				::std::numeric_limits<TScalar>::max()
			};
		}
	};

	template<typename TScalar>
	vector<TScalar> operator+(const vector<TScalar>& lhs, const vector<TScalar>& rhs) {
		return {
			lhs.x + rhs.x,
			lhs.y + rhs.y
		};
	};

	template<typename TScalar>
	vector<TScalar> operator-(const vector<TScalar>& lhs, const vector<TScalar>& rhs) {
		return {
			lhs.x - rhs.x,
			lhs.y - rhs.y
		};
	};

	template<typename TScalar>
	vector<TScalar> operator*(const vector<TScalar>& lhs, TScalar rhs) {
		return {
			lhs.x * rhs,
			lhs.y * rhs
		};
	};


	template<typename TScalar>
	bool is_inside(
		const vector<TScalar>& point,
		const vector<TScalar>& vertex) {
		return !(point.x < ::std::numeric_limits<TScalar>::denorm_min())
			&& !(point.y < ::std::numeric_limits<TScalar>::denorm_min())
			&& point.x < vertex.x
			&& point.y < vertex.y;
	};
	template<typename TScalar>
	bool is_overlapping(
		const vector<TScalar>& lhs_point, const vector<TScalar>& lhs_vertex,
		const vector<TScalar>& rhs_point, const vector<TScalar>& rhs_vertex) {
		vector<TScalar> abs_vertex{ lhs_point + lhs_vertex };
		if (!is_inside(rhs_point, abs_vertex)) {
			return false;
		}
		abs_vertex = rhs_point + rhs_vertex;
		if (!is_inside(lhs_point, abs_vertex)) {
			return false;
		}
		return true;
	}

	using vectord = vector<double>;
}