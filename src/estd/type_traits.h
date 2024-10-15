#pragma once

#include <type_traits>

namespace estd {
	template<typename R, typename T>
	constexpr bool is_invocable() {
		return std::_Is_invocable_r<R, T>::value;
	}

	template<typename Base, typename Derived>
	constexpr bool is_base_of() {
		return std::is_base_of<Base, Derived>::value;
	}

	template<typename T>
	struct remove_cvref {
		using type = typename std::remove_cv_t<std::remove_reference_t<T>>;
	};

	template<typename T>
	using remove_cvref_t = typename remove_cvref<T>::type;
}