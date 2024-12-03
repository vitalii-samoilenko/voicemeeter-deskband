#pragma once

#include "type_traits.h"

namespace estd {
	template<typename Func>
	class guard final {
		static_assert(
			is_invocable_r<void, Func>(),
			"Func must be invocable with no arguments and void return type");

	public:
		inline explicit guard(const Func& release) noexcept
			: m_release{ release } {

		};
		guard(const guard&) = delete;
		guard(guard&&) noexcept = default;

		inline ~guard() {
			m_release();
		};

		guard& operator=(const guard&) = delete;
		guard& operator=(guard&&) noexcept = default;

	private:
		Func m_release;
	};

	template<typename Func>
	inline guard<Func> make_guard(const Func& release) noexcept {
		return guard<Func>{ release };
	}
}