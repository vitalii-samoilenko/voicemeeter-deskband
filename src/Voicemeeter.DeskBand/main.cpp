#include <windows.h>

#include "estd/guard.h"
#include "Windows/Presentation/drawingengine.h"
#include "Windows/mainwindow.h"
#include "Windows/stackpanel.h"
#include "Windows/circularstatecontrol.h"
#include "Windows/error.h"
#include "errormessagebox.h"
#include "messages.h"

using namespace Voicemeeter::DeskBand::Windows;
using namespace Voicemeeter::DeskBand::Windows::Presentation;

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	DrawingEngine* pDrwEngine{ nullptr };
	MainWindow* pWndMain{ nullptr };
	auto mainGuard = estd::make_guard([&pDrwEngine , &pWndMain]()->void {
		delete pDrwEngine;
		delete pWndMain;
	});
	try {
		pDrwEngine = new DrawingEngine(Style::Default());
		pWndMain = new MainWindow{ hInstance };

		Panel* pPnlMain{ pWndMain->MakePanel<StackPanel<orientation::right>>() };
		pPnlMain->MakeControl<CircularStateControl>(std::make_pair(1U, 1U), 2L);
		pPnlMain->MakeControl<CircularStateControl>(std::make_pair(1U, 1U), 2L);
		pPnlMain->MakeControl<CircularStateControl>(std::make_pair(0U, 1U), 2L);

		pWndMain->Initialize();
		pWndMain->Show(nShowCmd);
	}
	catch (const windows_error& e) {
		ErrorMessageBox(e.code());

		return -1;
	}
	catch (const com_error& e) {
		ErrorMessageBox(e.code());

		return -1;
	}
	catch (...) {
		ErrorMessageBox(MSG_ERR_GENERAL);

		return -1;
	}

	MSG msg{};
	while (GetMessageW(&msg, NULL, 0, 0) != FALSE) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}