#pragma once

#include "windows.h"

#include "Environment/ITimer.h"

namespace Windows {
	class Timer : public ::Environment::ITimer {
	public:
		Timer(HWND hWnd);
		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;

		~Timer() = default;

		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) = delete;

		inline UINT_PTR get_Id() const {
			return reinterpret_cast<UINT_PTR>(this);
		};

		virtual void Set(::std::chrono::milliseconds duration, const ::std::function<bool()>& callback) override;
		virtual void Elapse() override;

	private:
		HWND m_hWnd;
		::std::function<bool()> m_callback;
	};
}