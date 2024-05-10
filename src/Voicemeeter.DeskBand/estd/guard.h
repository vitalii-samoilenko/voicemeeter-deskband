#pragma once

#include "isinvocable.h"

namespace estd {
	template<typename Func>
	class guard {
		static_assert(Is_invocable<void, Func>(),
			"Func type must be invocable with no arguments and void return type");

	public:
		explicit guard(Func release) noexcept;
		guard(const guard&) = delete;
		guard(guard&&) noexcept = default;

		~guard();

		guard& operator=(const guard&) = delete;
		guard& operator=(guard&&) noexcept = default;

	template<typename Func>
	guard<Func>::guard(Func release) noexcept
		: m_release{ release } {

	}

	template<typename Func>
	guard<Func>::~guard() {
		m_release();
	}

	template<typename Func>
	inline guard<Func> make_guard(Func release) noexcept {
		return guard<Func>{ release };
	}

	private:
		Func m_release;
	};
}