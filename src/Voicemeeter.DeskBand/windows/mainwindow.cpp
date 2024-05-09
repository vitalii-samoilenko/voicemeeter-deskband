#include "mainwindow.h"

#include "wrappers.h"
#include "resultcodes.h"

using namespace Voicemeeter::DeskBand::Windows;

std::once_flag MainWindow::WndMainClassGuard{};

LRESULT MainWindow::OnSize(UINT w, UINT h) {
	if (m_pPnl) {
		wMoveWindow(m_pPnl->get_hWnd(), 0U, 0U, w, h, FALSE);
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
  , m_pPnl{ nullptr } {

	}

void MainWindow::Initialize() {
	Window::Initialize(
		WS_OVERLAPPEDWINDOW,
		NULL
	);
	if (m_pPnl) {
		m_pPnl->Initialize();
	}
}
void MainWindow::Show(int nCmdShow) {
	Window::Show(nCmdShow);
	if (m_pPnl) {
		m_pPnl->Show(SW_SHOWDEFAULT);
	}
	wUpdateWindow(get_hWnd());
}