#include "checkbox.h"

#include "../estd/guard.h"
#include "wrappers.h"
#include "comerror.h"
#include "resultcodes.h"

using namespace Voicemeeter::DeskBand::Windows;

std::once_flag Checkbox::CtlCheckboxClassGuard{};

LRESULT Checkbox::OnLButtonDown(int x, int y) {
	FLOAT dpiX{};
	FLOAT dpiY{};
	get_rDrwEngine()
		.get_rPalette(*this)
		.pTarget->GetDpi(&dpiX, &dpiY);

	if (get_rScene()
			.Inside(x * USER_DEFAULT_SCREEN_DPI / dpiX, y * USER_DEFAULT_SCREEN_DPI / dpiY)
	) {
		m_checked = !m_checked;

		wInvalidateRect(get_hWnd(), NULL, FALSE);
	}

	return LRESULT_CODES::OK;
}
LRESULT Checkbox::OnPaint() {
	PAINTSTRUCT ps{};
	wBeginPaint(get_hWnd(), &ps);
	auto paintGuard = estd::make_guard([this, &ps]()->void {
		EndPaint(get_hWnd(), &ps);
		});

	if (ThrowIfFailed(get_rScene()
		.Draw(
			get_rDrwEngine()
			.get_rPalette(*this),
			m_checked
		), "Drawing failure", D2DERR_RECREATE_TARGET)) {
		get_rDrwEngine()
			.Initialize(*this);
	}

	return LRESULT_CODES::OK;
}

Checkbox::Checkbox(
	Panel& rPnlParent,
	DrawingEngine& rDrwEngine,
	LPCWSTR lpszLabel
) : Control{
		CtlCheckboxClassGuard,
		L"Voicemeeter.DeskBand.Checkbox",
		rPnlParent,
		rDrwEngine
	}
  , m_checked{ false }
  , m_scene{ lpszLabel } {

	}