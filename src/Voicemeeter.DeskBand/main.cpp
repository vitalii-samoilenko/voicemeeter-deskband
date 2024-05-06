#include <windows.h>

#include "estd/guard.h"
#include "windows/mainwindow.h"
#include "windows/stackpanel.h"
#include "windows/checkbox.h"
#include "windows/windowserror.h"
#include "windows/comerror.h"
#include "errormessagebox.h"
#include "messages.h"

using namespace Voicemeeter::DeskBand::Windows;

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
		pPnlMain->MakeControl<Checkbox, DrawingEngine&>(std::make_pair(1U, 1U), *pDrwEngine, L"A");
		pPnlMain->MakeControl<Checkbox, DrawingEngine&>(std::make_pair(1U, 1U), *pDrwEngine, L"B");
		pPnlMain->MakeControl<Checkbox, DrawingEngine&>(std::make_pair(0U, 1U), *pDrwEngine, L"SLIDER");

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