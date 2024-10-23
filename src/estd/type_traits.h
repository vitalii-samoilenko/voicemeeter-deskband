#pragma once

#include <iterator>
#include <type_traits>

namespace estd {
	template<typename T>
	using iterator_value_type_t = typename ::std::iterator_traits<T>::value_type;

	template<typename T>
	using remove_cvref_t = typename ::std::_Remove_cvref_t<T>;

	template<typename R, typename Func, typename... Args>
	using is_invocable_r = ::std::_Is_invocable_r<R, Func, Args...>;
}