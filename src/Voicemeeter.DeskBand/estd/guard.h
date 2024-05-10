#pragma once

#include "isinvocable.h"

namespace estd {
	template<typename Func>
	class guard {
		static_assert(Is_invocable<void, Func>(),
			"Func type must be invocable with no arguments and void return type");

	public:
		inline explicit guard(Func release) noexcept
			: m_release{ release } {

		}
		guard(const guard&) = delete;
		guard(guard&&) noexcept = default;

		inline ~guard() {
			m_release();
		}

		guard& operator=(const guard&) = delete;
		guard& operator=(guard&&) noexcept = default;

	private:
		Func m_release;
	};

	template<typename Func>
	inline guard<Func> make_guard(Func release) noexcept {
		return guard<Func>{ release };
	}
}