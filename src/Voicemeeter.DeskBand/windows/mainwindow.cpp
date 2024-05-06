#include "mainwindow.h"

#include <utility>

#include "wrappers.h"
#include "resultcodes.h"
#include "../messages.h"

#include "stackpanel.h"

using namespace Voicemeeter::DeskBand::Windows;

std::once_flag MainWindow::WndMainClassGuard{};

LRESULT MainWindow::OnSize() {
	if (m_pPanel) {
		RECT rc{};
		wGetClientRect(get_hWnd(), &rc);
		wMoveWindow(m_pPanel->get_hWnd(), rc.left, rc.top, rc.right, rc.bottom, FALSE);
		wInvalidateRect(get_hWnd(), NULL, FALSE);
		wUpdateWindow(get_hWnd());
	}

	return LRESULT_CODES::OK;
}
LRESULT MainWindow::OnDestroy() {
	PostQuitMessage(0);

	return LRESULT_CODES::OK;
}

MainWindow::MainWindow(
	HINSTANCE hInstance
) : Window{
		WndMainClassGuard,
		L"Voicemeeter.DeskBand.MainWindow",
		hInstance
	}
  , m_pPanel{ nullptr } {

	}

void MainWindow::Initialize() {
	Window::Initialize(
		WS_OVERLAPPEDWINDOW,
		NULL
	);
	if (m_pPanel) {
		m_pPanel->Initialize();
	}
}
void MainWindow::Show(int nCmdShow) {
	Window::Show(nCmdShow);
	if (m_pPanel) {
		m_pPanel->Show(SW_SHOWDEFAULT);
	}
	wUpdateWindow(get_hWnd());
}