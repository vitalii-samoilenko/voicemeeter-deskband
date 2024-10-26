#include <utility>

#include "window.h"
#include "windowsx.h"

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "ErrorMessageBox.h"

using namespace ::Voicemeeter::DeskBand::Windows;

static constexpr LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter.DeskBand" };
static constexpr DWORD EX_STYLE{ WS_EX_NOREDIRECTIONBITMAP };
static constexpr DWORD STYLE{ WS_OVERLAPPEDWINDOW };

static constexpr LRESULT OK{ 0 };

Window::Window(
	HINSTANCE hInstance
) : m_hWnd{ NULL }
, m_dpi{ USER_DEFAULT_SCREEN_DPI }
, m_pScene{ nullptr }
, m_remote{} {
	wSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	RECT rc{};
	rc.bottom = 40;
	rc.right = 251;
	wAdjustWindowRectExForDpi(&rc, STYLE, FALSE, EX_STYLE, m_dpi);

	WNDCLASSW wndClass{};
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = LPSZ_CLASS_NAME;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClass.lpfnWndProc = WindowProcW;
	wndClass.hCursor = wLoadCursorW(NULL, IDC_ARROW);

	wRegisterClassW(&wndClass);
	wCreateWindowExW(
		NULL,
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

Window::~Window() {
	if (m_remote.VBVMR_Logout != NULL) {
		m_remote.VBVMR_Logout();
	}
}

void Window::Show(int nCmdShow) const {
	ShowWindow(
		m_hWnd,
		nCmdShow
	);
}

void Window::EnableInputTrack() {
	SetCapture(m_hWnd);
}
void Window::DisableInputTrack() {
	wReleaseCapture();
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
	try {
		Window* pWnd{ wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA) };

		switch (uMsg) {
		case WM_NCCREATE: {
			pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_hWnd = hWnd;
			pWnd->m_dpi = GetDpiForWindow(hWnd);

			pWnd->BuildScene();
			//pWnd->Connect();

			if (wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd))) {
				return FALSE;
			}
		} break;
		case WM_DESTROY: {
			PostQuitMessage(0);
		} return OK;
		case WM_SIZE: {
			pWnd->m_pScene->Resize({
				static_cast<double>(LOWORD(lParam)),
				static_cast<double>(HIWORD(lParam))
			});
		} return OK;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			wBeginPaint(hWnd, &ps);

			pWnd->m_pScene->Redraw(
				::linear_algebra::vectord::origin(),
				::linear_algebra::vectord::infinity()
			);

			EndPaint(hWnd, &ps);
		} return OK;
		case WM_LBUTTONDOWN: {
			pWnd->m_pScene->MouseLDown({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_LBUTTONDBLCLK: {
			pWnd->m_pScene->MouseLDouble({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_RBUTTONDOWN: {
			pWnd->m_pScene->MouseRDown({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_MOUSEWHEEL: {
			POINT point{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			wScreenToClient(hWnd, &point);

			pWnd->m_pScene->MouseWheel({
				static_cast<double>(point.x),
				static_cast<double>(point.y)
			}, GET_WHEEL_DELTA_WPARAM(wParam));
		} return OK;
		case WM_MOUSEMOVE: {
			pWnd->m_pScene->MouseMove({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_LBUTTONUP: {
			pWnd->m_pScene->MouseLUp({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
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