#include <memory>

#include "Windows/API.hpp"
#include "Windows/ErrorMessageBox.hpp"

#include "DeskBandit.hpp"

#include "Messages.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	::std::unique_ptr<DeskBandit> deskBandit{ nullptr };
	try {
		deskBandit = ::std::make_unique<
			DeskBandit>(hInstance);
		deskBandit->Show(nShowCmd);
	}
	catch (::Windows::Error const &e) {
		::Windows::ErrorMessageBox(NULL, CPT_ERROR, e.code());
		return -1;
	}
	catch (...) {
		::Windows::ErrorMessageBox(NULL, CPT_ERROR, MSG_ERR_GENERAL);
		return -1;
	}
	MSG msg{};
	while (::GetMessageW(&msg, NULL, 0, 0) != FALSE) {
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}
	return 0;
}
