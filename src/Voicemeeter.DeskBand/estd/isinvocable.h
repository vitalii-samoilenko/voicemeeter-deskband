#pragma once

#include <type_traits>

namespace estd {
	template<typename R, typename T>
	constexpr bool Is_invocable() {
		return std::_Is_invocable_r<R, T>::value;
	}
}