#pragma once

namespace linear_algebra {
	struct vector {
		int x;
		int y;

		vector() = default;
		vector(const vector&) = default;
		vector(vector&&) = default;

		~vector() = default;

		vector& operator=(const vector&) = default;
		vector& operator=(vector&&) = default;
	};
}