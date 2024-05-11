#include "window.h"

#include <windowsx.h>

#include "../estd/guard.h"
#include "wrappers.h"
#include "resultcodes.h"
#include "../errormessagebox.h"

using namespace Voicemeeter::DeskBand::Windows;

static const LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter.DeskBand" };

Window::Window(HINSTANCE hInstance, Presentation::Scene& scene)
	: m_hInstance{ hInstance }
	, m_hWnd{ NULL }
	, m_scene{ scene } {
	WNDCLASSW wndClass{};
	wndClass.lpszClassName = LPSZ_CLASS_NAME;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProcW;
	wndClass.hCursor = wLoadCursorW(NULL, IDC_ARROW);
	wRegisterClassW(&wndClass);
}

void Window::Initialize() {
	wCreateWindowExW(
		0,
		LPSZ_CLASS_NAME,
		NULL,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		m_hInstance,
		this
	);
	m_scene.Initialize(m_hWnd);
}
void Window::Show(int nCmdShow) {
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
	Window* pWnd{ nullptr };
	if (uMsg == WM_NCCREATE) {
		pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
		pWnd->m_hWnd = hWnd;

		if (wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd))) {
			return FALSE;
		}
	} else {
		auto shutdown = [](DWORD errCode)->LRESULT {
			ErrorMessageBox(errCode);
			PostQuitMessage(0);

			return LRESULT_CODES::OK;
		};
		try {
			pWnd = wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA);

			if (pWnd) {
				switch (uMsg) {
				case WM_DESTROY:
					PostQuitMessage(0);

					return LRESULT_CODES::OK;
				case WM_SIZE: {
					RECT r{ 0L, 0L, LOWORD(lParam), HIWORD(lParam) };
					pWnd->m_scene.Resize(r.right, r.bottom);
					wInvalidateRect(pWnd->m_hWnd, &r, FALSE);

					return LRESULT_CODES::OK;
				}
				case WM_PAINT: {
					PAINTSTRUCT ps{};
					wBeginPaint(pWnd->m_hWnd, &ps);
					auto paintGuard = estd::make_guard([pWnd, &ps]()->void {
						EndPaint(pWnd->m_hWnd, &ps);
						});
					pWnd->m_scene.Draw();

					return LRESULT_CODES::OK;
				}
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
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}