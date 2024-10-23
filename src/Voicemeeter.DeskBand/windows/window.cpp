#include "window.h"

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"
#include "errormessagebox.h"

using namespace ::Voicemeeter::DeskBand::Windows;

static constexpr LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter.DeskBand" };
static constexpr DWORD EX_STYLE{ WS_EX_NOREDIRECTIONBITMAP };
static constexpr DWORD STYLE{ WS_OVERLAPPEDWINDOW };

static constexpr LRESULT OK{ 0 };

Window::Window(HINSTANCE hInstance)
	: m_hWnd{ NULL }
	, m_dpi{ USER_DEFAULT_SCREEN_DPI } {
	wSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	RECT rc{};
	rc.bottom = 40;
	rc.right = 251;
	wAdjustWindowRectExForDpi(&rc, STYLE, FALSE, EX_STYLE, m_dpi);

	WNDCLASSW wndClass{};
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = LPSZ_CLASS_NAME;
	wndClass.lpfnWndProc = WindowProcW;
	wndClass.hCursor = wLoadCursorW(NULL, IDC_ARROW);
	wRegisterClassW(&wndClass);
	wCreateWindowExW(
		EX_STYLE,
		LPSZ_CLASS_NAME,
		NULL,
		STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		this
	);
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

		return OK;
	};
	try{
		Window* pWnd{ wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA) };

		switch (uMsg) {
		case WM_NCCREATE: {
			pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_hWnd = hWnd;
			pWnd->m_dpi = GetDpiForWindow(hWnd);

			if (wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd))) {
				return FALSE;
			}
		} break;
		case WM_DESTROY: {
			PostQuitMessage(0);
		} return OK;
		case WM_SIZE: {
			//pWnd->m_scene.Resize(LOWORD(lParam), HIWORD(lParam));
		} return OK;
		case WM_GETDPISCALEDSIZE: {
			const FLOAT scale{ static_cast<FLOAT>(wParam) / pWnd->m_dpi };

			RECT rc{};
			wGetClientRect(hWnd, &rc);
			rc.right *= scale;
			rc.bottom *= scale;
			wAdjustWindowRectExForDpi(&rc, STYLE, FALSE, EX_STYLE, wParam);

			LPSIZE pSize{ reinterpret_cast<LPSIZE>(lParam) };
			pSize->cx = rc.right - rc.left;
			pSize->cy = rc.bottom - rc.top;

			pWnd->m_dpi = wParam;
		} return TRUE;
		case WM_DPICHANGED: {
			const LPRECT pRc{ reinterpret_cast<LPRECT>(lParam) };

			wSetWindowPos(
				hWnd, NULL,
				pRc->left, pRc->top,
				pRc->right - pRc->left, pRc->bottom - pRc->top,
				0U
			);
		} return OK;
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