#include "panel.h"

#include "windowserror.h"
#include "resultcodes.h"
#include "../messages.h"

using namespace Voicemeeter::DeskBand::Windows;

LRESULT Control::OnCreate() {
	get_rDrwEngine()
		.Initialize(*this);

	return LRESULT_CODES::OK;
}
LRESULT Control::OnSize() {
	get_rScene()
		.Resize(
			get_rDrwEngine()
				.Resize(*this)
		);

	return LRESULT_CODES::OK;
}

Control::Control(
	std::once_flag& rWndClassGuard,
	LPCWSTR lpszClassName,
	Panel& rPnlParent,
	DrawingEngine& rDrwEngine
) : Window{
		rWndClassGuard,
		lpszClassName,
		rPnlParent.get_hInstance()
	}
  , m_rPnlParent{ rPnlParent }
  , m_rDrwEngine{ rDrwEngine } {

	}

void Control::Initialize() {
	Window::Initialize(
		WS_CHILD,
		m_rPnlParent.get_hWnd()
	);
}