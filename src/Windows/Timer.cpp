#include "Wrappers.h"

#include "Timer.h"

using namespace Windows;

Timer::Timer(
	HWND hWnd
) : m_hWnd{ hWnd }
  , m_callback{} {

}

void Timer::Set(::std::chrono::milliseconds duration, const ::std::function<bool()>& callback) {
	wSetTimer(m_hWnd, get_Id(), static_cast<UINT>(duration.count()), NULL);
	m_callback = callback;
}

void Timer::Elapse() {
	if (m_callback) {
		if (m_callback()) {
			return;
		}
		wKillTimer(m_hWnd, get_Id());
		m_callback = {};
	}
}