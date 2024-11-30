#pragma once

#include <iterator>
#include <type_traits>

namespace estd {
	template<typename T>
	using iterator_value_type_t = typename ::std::iterator_traits<T>::value_type;

	template<typename R, typename Func, typename... Args>
	using is_invocable_r = ::std::_Is_invocable_r<R, Func, Args...>;

	template<typename TL, typename TR>
	constexpr bool are_same() {
	    return ::std::is_same_v<TL, TR>;
	}
	
	template<typename TL, typename TR, typename TN, typename... Args>
	constexpr bool are_same() {
	    return ::std::is_same_v<TL, TR> && are_same<TL, TN, Args...>();
	}
}