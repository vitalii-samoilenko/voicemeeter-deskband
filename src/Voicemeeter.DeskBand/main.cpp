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
	std::unique_ptr<DrawingEngine> pDrwEngine{ nullptr };
	std::unique_ptr<Scene> pScene{ nullptr };
	std::unique_ptr<Window> pWnd{ nullptr };
	try {
		pDrwEngine.reset(new DrawingEngine(style));
		pScene.reset(new Scene(*pDrwEngine));
		pWnd.reset(new Window{ hInstance, *pScene });

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