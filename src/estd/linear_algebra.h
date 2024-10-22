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

	bool is_inside(vector point, vector vertex);
	bool is_overlapping(vector lhs_point, vector lhs_vertex, vector rhs_point, vector rhs_vertex);
}