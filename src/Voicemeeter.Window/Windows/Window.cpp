#include <cmath>
#include <fstream>
#include <string>
#include <utility>

#include "Voicemeeter.Scene.D2D.Remote/Build.h"
#include "Voicemeeter.UI/Direction.h"
#include "Windows/ErrorMessageBox.h"
#include "Windows/Registry.h"
#include "Windows/Wrappers.h"

#include "Window.h"

using namespace ::Voicemeeter::Windows;

static constexpr LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter" };
static constexpr DWORD STYLE{ WS_OVERLAPPEDWINDOW };

static constexpr LRESULT OK{ 0 };

Window::Window(
	HINSTANCE hInstance
) : m_hWnd{ NULL }
  , m_dpi{ USER_DEFAULT_SCREEN_DPI }
  , m_pCompositionTimer{ nullptr }
  , m_pGraphicsTimer{ nullptr }
  , m_pMixerTimer{ nullptr }
  , m_lpTimer{}
  , m_pMixer{ nullptr }
  , m_pScene{ nullptr } {
	::Windows::wSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	RECT rc{};
	rc.bottom = 48;
	rc.right = 592;
	::Windows::wAdjustWindowRectExForDpi(&rc, STYLE, FALSE, NULL, m_dpi);

	WNDCLASSW wndClass{};
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = LPSZ_CLASS_NAME;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = WndProcW;
	wndClass.hCursor = ::Windows::wLoadCursorW(NULL, IDC_ARROW);

	::Windows::wRegisterClassW(&wndClass);
	::Windows::wCreateWindowExW(
		WS_EX_NOREDIRECTIONBITMAP,
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

void Window::EnableInputTrack() {
	SetCapture(m_hWnd);
}
void Window::DisableInputTrack() {
	::Windows::wReleaseCapture();
}

LRESULT CALLBACK Window::WndProcW(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	auto shutdown = [uMsg](long long errCode)->LRESULT {
		if (uMsg == WM_NCCREATE) {
			return FALSE;
		}

		::Windows::ErrorMessageBox(errCode);
		PostQuitMessage(0);

		return OK;
	};
	try {
		Window* pWnd{ ::Windows::wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA) };
		switch (uMsg) {
		case WM_NCCREATE: {
			pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_dpi = GetDpiForWindow(hWnd);
			pWnd->m_pCompositionTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pGraphicsTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pMixerTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_lpTimer.emplace(pWnd->m_pCompositionTimer->get_Id(), pWnd->m_pCompositionTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pGraphicsTimer->get_Id(), pWnd->m_pGraphicsTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pMixerTimer->get_Id(), pWnd->m_pMixerTimer.get());
			pWnd->m_pMixer.reset(new ::Voicemeeter::Remote::Mixer(*pWnd->m_pMixerTimer));
			pWnd->m_pScene.reset(::Voicemeeter::Scene::D2D::Remote::Build(
				hWnd, UI::Direction::Right, *pWnd,
				*pWnd->m_pCompositionTimer, *pWnd->m_pGraphicsTimer,
				*pWnd->m_pMixer));
			::Windows::wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			pWnd->m_hWnd = hWnd;
		} break;
		case WM_DESTROY: {
			PostQuitMessage(0);
		} return OK;
		case WM_TIMER: {
			pWnd->m_lpTimer[static_cast<UINT_PTR>(wParam)]
				->Elapse();
		} return OK;
		case WM_SIZE: {
			pWnd->m_pScene->Resize({
				static_cast<double>(LOWORD(lParam)),
				static_cast<double>(HIWORD(lParam))
			});
			pWnd->m_pScene->Redraw(
				pWnd->m_pScene->get_Position(),
				pWnd->m_pScene->get_Size()
			);
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
		case WM_MBUTTONDOWN: {
			pWnd->m_pScene->MouseMDown({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_MBUTTONDBLCLK: {
			pWnd->m_pScene->MouseMDouble({
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
		case WM_RBUTTONDBLCLK: {
			pWnd->m_pScene->MouseRDouble({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_MOUSEWHEEL: {
			POINT point{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::Windows::wScreenToClient(hWnd, &point);

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
			::Windows::wGetClientRect(hWnd, &rc);
			rc.right = static_cast<LONG>(::std::ceil(rc.right * scale));
			rc.bottom = static_cast<LONG>(::std::ceil(rc.bottom * scale));
			::Windows::wAdjustWindowRectExForDpi(&rc, STYLE, FALSE, NULL, static_cast<UINT>(wParam));

			LPSIZE pSize{ reinterpret_cast<LPSIZE>(lParam) };
			pSize->cx = rc.right - rc.left;
			pSize->cy = rc.bottom - rc.top;

			pWnd->m_dpi = static_cast<UINT>(wParam);
		} return TRUE;
		case WM_DPICHANGED: {
			const LPRECT pRc{ reinterpret_cast<LPRECT>(lParam) };

			::Windows::wSetWindowPos(
				hWnd, NULL,
				pRc->left, pRc->top,
				pRc->right - pRc->left, pRc->bottom - pRc->top,
				0U
			);
		} return OK;
		}
	}
	catch (const ::Windows::Error& e) {
		WCHAR temp[MAX_PATH];
		if (GetTempPathW(MAX_PATH, temp)) {
			::std::wstring path{ temp };
			::std::fstream log{ path.append(L"Voicemeeter.DeskBand.log"), log.out | log.app };
			if (log.is_open()) {
				log << e.what() << ::std::endl;
			}
		}

		return shutdown(e.code());
	}
	catch (...) {
		return shutdown(MSG_ERR_GENERAL);
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}