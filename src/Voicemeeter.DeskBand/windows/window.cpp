#include "window.h"

#include <windowsx.h>

#include "wrappers.h"
#include "resultcodes.h"
#include "../errormessagebox.h"

using namespace Voicemeeter::DeskBand::Windows;

LRESULT Window::OnCreate() {
	return LRESULT_CODES::OK;
}
LRESULT Window::OnDestroy() {
	return LRESULT_CODES::OK;
}
LRESULT Window::OnPaint() {
	return LRESULT_CODES::OK;
}
LRESULT Window::OnSize(UINT w, UINT h) {
	return LRESULT_CODES::OK;
}
LRESULT Window::OnLButtonDown(LONG x, LONG y) {
	return LRESULT_CODES::OK;
}

Window::Window(
	std::once_flag& rWndClassGuard,
	LPCWSTR lpszClassName,
	HINSTANCE hInstance
) : m_lpszClassName{ lpszClassName }
  , m_hWnd{ NULL }
  , m_hInstance{ hInstance } {
		std::call_once(rWndClassGuard, [lpszClassName, hInstance]()->void {
			WNDCLASSW wndClass{};
			wndClass.lpszClassName = lpszClassName;
			wndClass.hInstance = hInstance;
			wndClass.lpfnWndProc = WindowProcW;
			wRegisterClassW(&wndClass);
		});
	}

void Window::Initialize(
	DWORD dwStyle,
	HWND hWndParent
) {
	wCreateWindowExW(
		0,
		m_lpszClassName,
		NULL,
		dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hWndParent,
		NULL,
		get_hInstance(),
		this
	);
}

LRESULT CALLBACK Window::WindowProcW(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	Window* pWnd{ nullptr };
	if (uMsg == WM_NCCREATE) {
		pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
		pWnd->m_hWnd = hWnd;

		if (wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd))) {
			return FALSE;
		}
	}
	else {
		auto shutdown = [](DWORD errCode)->LRESULT {
			ErrorMessageBox(errCode);
			PostQuitMessage(0);

			return LRESULT_CODES::OK;
		};
		try {
			pWnd = wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA);

			if (pWnd) {
				switch (uMsg) {
				case WM_CREATE:
					return pWnd->OnCreate();
				case WM_DESTROY:
					return pWnd->OnDestroy();
				case WM_PAINT:
					return pWnd->OnPaint();
				case WM_SIZE:
					return pWnd->OnSize(LOWORD(lParam), HIWORD(lParam));
				case WM_LBUTTONDOWN:
					return pWnd->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				}
			}
		}
		catch (const handler_error& e) {
			ErrorMessageBox(e.code());

			return e.result();
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
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void Window::Show(int nCmdShow) {
	ShowWindow(
		get_hWnd(),
		nCmdShow
	);
}