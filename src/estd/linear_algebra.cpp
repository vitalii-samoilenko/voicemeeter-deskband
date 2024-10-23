#include <limits>

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

vector Origin{};
vector Infinity{
	::std::numeric_limits<int>::max(),
	::std::numeric_limits<int>::max()
};

bool linear_algebra::is_inside(
	const vector& point,
	const vector& vertex) {
	return point.x - vertex.x < 0
		&& point.y - vertex.y < 0;
}

bool linear_algebra::is_overlapping(
	const vector& lhs_point, const vector& lhs_vertex,
	const vector& rhs_point, const vector& rhs_vertex) {
	vector abs_vertex{ lhs_point + lhs_vertex };
	if (!is_inside(rhs_point, abs_vertex)) {
		return false;
	}
	abs_vertex = rhs_point + rhs_vertex;
	if (!is_inside(lhs_point, abs_vertex)) {
		return false;
	}
	return true;
}