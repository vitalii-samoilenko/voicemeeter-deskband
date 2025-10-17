#ifndef ESTD_GUARD_HPP
#define ESTD_GUARD_HPP

#include <type_traits>
#include <utility>

namespace estd {
	template<typename Fn>
	class guard final {
		static_assert(
			::std::is_invocable_r_v<void, Fn>,
			"Fn must be invocable with no arguments and void return type");

	public:
		inline explicit guard(Fn const &release) noexcept
			: _release{ release } {

		};
		guard(guard const&) = delete;
		inline guard(guard &&) noexcept = default;

		inline ~guard() {
			_release();
		};

		guard & operator=(guard const &) = delete;
		inline guard & operator=(guard &&) noexcept = default;

	private:
		Fn _release;
	};

	template<typename Fn>
	inline guard<Fn> make_guard(Fn &&release) noexcept {
		return guard<Fn>{ ::std::forward<Fn>(release) };
	};
}

#endif
