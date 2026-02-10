#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <cmath>
#include <valarray>

using num_t = int32_t;

#define _PRECISION 6
#define _MAX_SQRT 46340

inline constexpr num_t push(num_t n) {
	return n << _PRECISION;
};
inline constexpr num_t pop(num_t n) {
	return n >> _PRECISION;
};
inline constexpr bool is_safe(num_t n) {
	return -(_MAX_SQRT + 1) < n
		&& n < _MAX_SQRT + 1;
};

constexpr num_t One{ push(1) };
constexpr num_t Inf{ _MAX_SQRT * _MAX_SQRT + 1 };

inline constexpr num_t floor(num_t n) {
	return pop(n);
};
inline constexpr num_t ceil(num_t n) {
	return floor(n + One - 1);
};

inline num_t ans(num_t n) {
	return ::std::abs(n);
};
inline num_t sqrt(num_t n) {
	return static_cast<num_t>(
		::std::sqrt(push(n)));
};
inline num_t sinI(num_t n) {
	return static_cast<num_t>(One
		/ ::std::sin(
			n * M_PI / push(180)));
};
inline num_t cosI(num_t n) {
	return static_cast<num_t>(One
		/ ::std::cos(
			n * M_PI / push(180)));
};

using vector_t = ::std::valarray<num_t>;
using slice_t = ::std::slice;

template<typename V>
inline auto push(V const &v) {
	return v << _PRECISION;
};
template<typename V>
inline auto pop(V const &v) {
	return v >> _PRECISION;
};
template<typename V>
inline num_t sum(V const &v) {
	return v.sum();
};
template<typename V>
inline num_t max(V const &v) {
	return v.max();
};
template<typename V>
inline num_t min(V const &v) {
	return v.min();
};
template<typename V>
inline num_t any(V const &v) {
	return max(v);
};
template<typename V>
inline num_t all(V const &v) {
	return min(v);
};

#endif
