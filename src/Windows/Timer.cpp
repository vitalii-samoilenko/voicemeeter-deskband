#include "Wrappers.h"

#include "Timer.h"

using namespace Windows;

Timer::Timer(
	HWND hWnd
) : m_hWnd{ hWnd }
  , m_callback{} {

}

void Timer::Set(::std::chrono::milliseconds duration, const ::std::function<void()>& callback) {
	wSetTimer(m_hWnd, reinterpret_cast<UINT_PTR>(this), static_cast<UINT>(duration.count()), NULL);
	m_callback = callback;
}

void Timer::Elapse() {
	if (m_callback) {
		m_callback();
		m_callback = {};
	}
}