#pragma once

#include <iterator>
#include <type_traits>

namespace estd {
	template<typename R, typename T, typename... A>
	constexpr bool is_invocable() {
		return std::_Is_invocable_r<R, T, A...>::value;
	}

	template<typename Base, typename Derived>
	constexpr bool is_base_of() {
		return std::is_base_of<Base, Derived>::value;
	}

	template<typename T>
	using remove_cvref_t = typename std::_Remove_cvref_t<T>;

	template<typename T>
	using iterator_value_type_t = typename std::iterator_traits<T>::value_type;
}