#include "panel.h"

#include "wrappers.h"
#include "resultcodes.h"

using namespace Voicemeeter::DeskBand::Windows;

Panel::Panel(
	std::once_flag& rWndClassGuard,
	LPCWSTR lpszClassName,
	Window& rWndParent
) : Window{
		rWndClassGuard,
		lpszClassName,
		rWndParent.get_hInstance()
	}
  , m_rWndParent{ rWndParent }
  , m_cPack{} {

	}

void Panel::Initialize() {
	Window::Initialize(
		WS_CHILD,
		m_rWndParent.get_hWnd()
	);
	for (const pack_type& rPack : get_rcPack()) {
		std::get<pControl>(rPack)->Initialize();
	}
}
void Panel::Show(int nCmdShow) {
	Window::Show(nCmdShow);
	for (const pack_type& rPack : get_rcPack()) {
		std::get<pControl>(rPack)->Show(SW_SHOWDEFAULT);
	}
}