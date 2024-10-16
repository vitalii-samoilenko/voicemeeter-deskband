#include "linear_algebra.h"

using namespace linear_algebra;

vector linear_algebra::operator+(const vector& lhs, const vector& rhs) {
	return {
		lhs.x + rhs.x,
		lhs.y + rhs.y
	};
}

vector linear_algebra::operator-(const vector& lhs, const vector& rhs) {
	return {
		lhs.x - rhs.x,
		lhs.y - rhs.y
	};
}

bool linear_algebra::is_outside(vector point, vector vertex) {
	vector distance{ point - vertex };

	return distance.x < 0 || distance.y < 0;
}