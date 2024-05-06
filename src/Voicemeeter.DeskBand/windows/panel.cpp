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
  , m_packs{} {

	}

void Panel::Initialize() {
	Window::Initialize(
		WS_CHILD,
		m_rWndParent.get_hWnd()
	);
	for (pack_type& pack : m_packs) {
		std::get<pControl>(pack)->Initialize();
	}
}
void Panel::Show(int nCmdShow) {
	Window::Show(nCmdShow);
	for (const pack_type& pack : get_rPacks()) {
		std::get<pControl>(pack)->Show(SW_SHOWDEFAULT);
	}
}