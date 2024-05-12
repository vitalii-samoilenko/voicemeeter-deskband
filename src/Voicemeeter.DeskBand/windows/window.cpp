#include "window.h"

#include "wrappers.h"
#include "resultcodes.h"
#include "../errormessagebox.h"

using namespace Voicemeeter::DeskBand::Windows;

static const LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter.DeskBand" };

Window::Window(HINSTANCE hInstance, Presentation::Scene& scene)
	: m_hWnd{ NULL }
	, m_scene{ scene } {
	WNDCLASSW wndClass{
		0U,								//.style
		WindowProcW,					//.lpfnWndProc
		0, 0,							//.cbClsExtra, .cbWndExtra
		hInstance,						//.hInstance
		NULL,							//.hIcon
		wLoadCursorW(NULL, IDC_ARROW),	//.hCursor
		NULL,							//.hbrBackground
		NULL,							//.lpszMenuName
		LPSZ_CLASS_NAME					//.lpszClassName
	};
	wRegisterClassW(&wndClass);
	wCreateWindowExW(
		0,
		LPSZ_CLASS_NAME,
		NULL,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		this
	);
	m_scene.Initialize(m_hWnd);
}

void Window::Show(int nCmdShow) const {
	ShowWindow(
		m_hWnd,
		nCmdShow
	);
}

LRESULT CALLBACK Window::WindowProcW(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	auto shutdown = [](DWORD errCode)->LRESULT {
		ErrorMessageBox(errCode);
		PostQuitMessage(0);

		return LRESULT_CODES::OK;
	};
	try{
		Window* pWnd{ wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA) };

		switch (uMsg) {
		case WM_NCCREATE:
			pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_hWnd = hWnd;

			if (wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd))) {
				return FALSE;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);

			return LRESULT_CODES::OK;
		case WM_SIZE: {
			pWnd->m_scene.Resize(LOWORD(lParam), HIWORD(lParam));

			return LRESULT_CODES::OK;
		}
		}
	}
	catch (const windows_error& e) {
		return shutdown(e.code());
	}
	catch (const com_error& e) {
		return shutdown(e.code());
	}
	catch (...) {
		return shutdown(MSG_ERR_GENERAL);
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}