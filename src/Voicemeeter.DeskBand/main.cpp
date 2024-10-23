#include <memory>

#include <windows.h>

#include "Windows/window.h"
#include "Voicemeeter.DeskBand.Windows/error.h"
#include "Voicemeeter.DeskBand.Windows/wrappers.h"
#include "Windows/errormessagebox.h"

using namespace ::Voicemeeter::DeskBand::Windows;

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	::std::unique_ptr<Window> pWnd{ nullptr };
	try {
		pWnd.reset(new Window{ hInstance });

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