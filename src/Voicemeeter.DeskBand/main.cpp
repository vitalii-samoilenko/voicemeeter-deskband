#include <windows.h>

#include "estd/guard.h"
#include "Windows/Presentation/drawingengine.h"
#include "Windows/window.h"
#include "Windows/error.h"
#include "errormessagebox.h"

using namespace Voicemeeter::DeskBand::Windows;
using namespace Voicemeeter::DeskBand::Windows::Presentation;

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	DrawingEngine* pDrwEngine{ nullptr };
	Window* pWnd{ nullptr };
	auto mainGuard = estd::make_guard([&pDrwEngine , &pWnd]()->void {
		delete pDrwEngine;
		delete pWnd;
	});
	try {
		pDrwEngine = new DrawingEngine(Style::Default());
		pWnd = new Window{ hInstance };

		pWnd->Initialize();
		pWnd->Show(nShowCmd);
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