#include "panel.h"

#include "../estd/guard.h"
#include "wrappers.h"
#include "resultcodes.h"
#include "../messages.h"

using namespace Voicemeeter::DeskBand::Windows;

LRESULT Control::OnCreate() {
	return LRESULT_CODES::OK;
}
LRESULT Control::OnSize(UINT w, UINT h) {
	return LRESULT_CODES::OK;
}
LRESULT Control::OnPaint() {
	PAINTSTRUCT ps{};
	wBeginPaint(get_hWnd(), &ps);
	auto paintGuard = estd::make_guard([this, &ps]()->void {
		EndPaint(get_hWnd(), &ps);
	});

	return LRESULT_CODES::OK;
}

Control::Control(
	std::once_flag& rWndClassGuard,
	LPCWSTR lpszClassName,
	Panel& rPnlParent
) : Window{
		rWndClassGuard,
		lpszClassName,
		rPnlParent.get_hInstance()
	}
  , m_rPnlParent{ rPnlParent } {

	}

void Control::Initialize() {
	Window::Initialize(
		WS_CHILD,
		m_rPnlParent.get_hWnd()
	);
}