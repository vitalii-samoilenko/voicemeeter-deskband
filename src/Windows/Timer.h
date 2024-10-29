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

		virtual void Set(::std::chrono::milliseconds duration, const ::std::function<void()>& callback) override;
		virtual void Elapse();

	private:
		HWND m_hWnd;
		::std::function<void()> m_callback;
	};
}