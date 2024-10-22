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

bool linear_algebra::is_inside(vector point, vector vertex) {
	vector distance{ point - vertex };

	return distance.x < 0 && distance.y < 0;
}

bool linear_algebra::is_overlapping(vector lhs_point, vector lhs_vertex, vector rhs_point, vector rhs_vertex) {
	linear_algebra::vector abs_vertex{ lhs_point + lhs_vertex };
	if (!linear_algebra::is_inside(rhs_point, abs_vertex)) {
		return false;
	}
	abs_vertex = rhs_point + rhs_vertex;
	if (!linear_algebra::is_inside(lhs_point, abs_vertex)) {
		return false;
	}

	return true;
}