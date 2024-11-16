#pragma once

#include <limits>

#include "type_traits.h"

namespace linear_algebra {
	template<typename T>
	struct vector final {
		static_assert(
			::std::is_arithmetic_v<T>,
			"T must be of arithmetic type");

		T x;
		T y;

		vector() = default;
		vector(const vector&) = default;
		vector(vector&&) = default;

		~vector() = default;

		vector& operator=(const vector&) = default;
		vector& operator=(vector&&) = default;

		static constexpr vector origin() {
			return {};
		};

		static constexpr vector infinity() {
			return {
				::std::numeric_limits<T>::max(),
				::std::numeric_limits<T>::max()
			};
		}
	};

	template<typename T>
	vector<T> operator+(const vector<T>& lhs, const vector<T>& rhs) {
		return {
			lhs.x + rhs.x,
			lhs.y + rhs.y
		};
	};

	template<typename T>
	vector<T> operator-(const vector<T>& lhs, const vector<T>& rhs) {
		return {
			lhs.x - rhs.x,
			lhs.y - rhs.y
		};
	};

	template<typename T>
	vector<T> operator*(const vector<T>& lhs, T rhs) {
		return {
			lhs.x * rhs,
			lhs.y * rhs
		};
	};


	template<typename T>
	bool is_inside(
		const vector<T>& point,
		const vector<T>& vertex) {
		return !(point.x < 0)
			&& !(point.y < 0)
			&& point.x < vertex.x
			&& point.y < vertex.y;
	};
	template<typename T>
	bool is_overlapping(
		const vector<T>& lhs_point, const vector<T>& lhs_vertex,
		const vector<T>& rhs_point, const vector<T>& rhs_vertex) {
		vector<T> abs_vertex{ lhs_point + lhs_vertex };
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