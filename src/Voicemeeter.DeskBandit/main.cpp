#include <memory>

#include "Windows/Wrappers.h"
#include "Windows/ErrorMessageBox.h"

#include "Windows/DeskBandit.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	::std::unique_ptr<::Voicemeeter::Windows::DeskBandit> pWnd{ nullptr };
	try {
		pWnd.reset(new ::Voicemeeter::Windows::DeskBandit{ hInstance });

		pWnd->Show(nShowCmd);
	}
	catch (const ::Windows::Error& e) {
		::Windows::ErrorMessageBox(e.code());

		return -1;
	}
	catch (...) {
		::Windows::ErrorMessageBox(MSG_ERR_GENERAL);

		return -1;
	}

	MSG msg{};
	while (GetMessageW(&msg, NULL, 0, 0) != FALSE) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}