#ifndef WINDOWS_TIMER_HPP
#define WINDOWS_TIMER_HPP

#include <chrono>
#include <functional>
#include <utility>

#include "Windows/API.h"

namespace Windows {
	class Timer final {
	public:
		inline explicit Timer(HWND hWnd)
			: _hWnd{ hWnd }
			, _callback{} {

		};
		Timer(Timer const &) = delete;
		Timer(Timer &&) = delete;

		inline ~Timer() = default;

		Timer & operator=(Timer const &) = delete;
		Timer & operator=(Timer &&) = delete;

		inline UINT_PTR get_Id() const {
			return reinterpret_cast<UINT_PTR>(this);
		};

		template<typename Fn>
		inline void Set(::std::chrono::milliseconds duration, Fn &&callback) {
			SetTimer(_hWnd, get_Id(), static_cast<UINT>(duration.count()), NULL);
			_callback = ::std::forward<Fn>(callback);
		};
		inline void Elapse() {
			if (_callback()) {
				return;
			}
			_callback = {};
			KillTimer(_hWnd, get_Id());
		};

	private:
		HWND _hWnd;
		::std::function<bool()> _callback;
	};
}

#endif
