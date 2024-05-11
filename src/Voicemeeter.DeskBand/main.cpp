#include "estd/guard.h"
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
	Style style{ Style::Default() };
	DrawingEngine* pDrwEngine{ nullptr };
	Scene* pScene{ nullptr };
	Window* pWnd{ nullptr };
	auto mainGuard = estd::make_guard([&pDrwEngine, &pScene, &pWnd]()->void {
		delete pDrwEngine;
		delete pScene;
		delete pWnd;
	});
	try {
		pDrwEngine = new DrawingEngine(style);
		pScene = new Scene(*pDrwEngine);
		pWnd = new Window{ hInstance, *pScene };

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