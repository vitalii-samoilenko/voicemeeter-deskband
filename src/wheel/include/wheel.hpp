#ifndef WHEEL_HPP
#define WHEEL_HPP

#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <valarray>

#ifdef _WHEEL_EXTENDED_PRECISION
using num_t = int64_t;
using unum_t = uint64_t;
constexpr int _wheel_Precision{ 8 };
constexpr num_t _wheel_Msqrt{ 3037000499 };
#else
using num_t = int32_t;
using unum_t = uint32_t;
constexpr int _wheel_Precision{ 6 };
constexpr num_t _wheel_Msqrt{ 46340 };
#endif

template<typename T,
	::std::enable_if_t<
		::std::is_integral_v<T>,
		bool> = true>
inline constexpr num_t push(T n) {
	return static_cast<num_t>(n)
		<< _wheel_Precision;
};
template<typename T,
	::std::enable_if_t<
		::std::is_integral_v<T>,
		bool> = true>
inline constexpr num_t pop(T n) {
	return static_cast<num_t>(n)
		>> _wheel_Precision;
};

inline constexpr bool is_safe(num_t n) {
	return -(_wheel_Msqrt + 1) < n
		&& n < _wheel_Msqrt + 1;
};

constexpr num_t Zero{ 0 };
constexpr num_t One{ push(1) };
constexpr num_t Inf{ _wheel_Msqrt * _wheel_Msqrt + 1 };

constexpr unum_t _wheel_Imask{
	::std::numeric_limits<unum_t>::max()
		<< _wheel_Precision
};
inline constexpr num_t floor(num_t n) {
	return n & _wheel_Imask;
};
inline constexpr num_t ceil(num_t n) {
	return floor(n + One - 1);
};
inline constexpr num_t frac(num_t n) {
	return n & ~_wheel_Imask;
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

template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline auto push(V const &v) {
	return v << _wheel_Precision;
};
template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline auto pop(V const &v) {
	return v >> _wheel_Precision;
};
template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline num_t sum(V const &v) {
	return v.sum();
};
template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline num_t max(V const &v) {
	return v.max();
};
template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline num_t min(V const &v) {
	return v.min();
};
template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline num_t any(V const &v) {
	return max(v);
};
template<typename V,
	::std::enable_if_t<
		!::std::is_integral_v<V>,
		bool> = true>
inline num_t all(V const &v) {
	return min(v);
};
}

#endif
