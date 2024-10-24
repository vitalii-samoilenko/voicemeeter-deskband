#pragma once

namespace linear_algebra {
	struct vector final {
		int x;
		int y;

		vector() = default;
		vector(const vector&) = default;
		vector(vector&&) = default;

		~vector() = default;

		vector& operator=(const vector&) = default;
		vector& operator=(vector&&) = default;
	};

	vector operator+(const vector& lhs, const vector& rhs);
	vector operator-(const vector& lhs, const vector& rhs);

	extern const vector Origin;
	extern const vector Infinity;

	bool is_inside(
		const vector& point,
		const vector& vertex);
	bool is_overlapping(
		const vector& lhs_point, const vector& lhs_vertex,
		const vector& rhs_point, const vector& rhs_vertex);
}