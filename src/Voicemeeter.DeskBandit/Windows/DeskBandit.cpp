#include <cmath>
#include <fstream>
#include <string>

#include "Windows/ErrorMessageBox.h"
#include "Windows/Registry.h"
#include "Windows/Wrappers.h"

#include "DeskBandit.h"

using namespace ::Voicemeeter::Windows;

static constexpr LRESULT OK{ 0 };

DeskBandit::DeskBandit(
	HINSTANCE hInstance
) : m_hWndParent{ ::Windows::wFindWindowExW(NULL, NULL, L"Shell_TrayWnd", NULL) }
  , m_hWndTray{ ::Windows::wFindWindowExW(m_hWndParent, NULL, L"TrayNotifyWnd", NULL) }
  , m_rc{}
  , m_dock{ Dock::Left }
  , m_hWnd{ NULL }
  , m_pDockTimer{ nullptr }
  , m_pCompositionTimer{ nullptr }
  , m_pRenderTimer{ nullptr }
  , m_pRemoteTimer{ nullptr }
  , m_lpTimer{}
  , m_pMixer{ nullptr }
  , m_pRemote{ nullptr }
  , m_pScene{ nullptr }
  , m_pD2dScene{ nullptr }
  , m_pD3d12Scene{ nullptr }{
	::Windows::wSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	RECT parent{};
	::Windows::wGetWindowRect(m_hWndParent, &parent);
	m_rc.bottom = parent.bottom - parent.top;
	m_rc.right = parent.right - parent.left;

	DWORD dock{ 0UL };
	if (::Windows::Registry::TryGetValue(HKEY_CURRENT_USER, LR"(SOFTWARE\VoicemeeterDeskBand)", L"Dock", dock)
			&& dock == static_cast<DWORD>(Dock::Right)) {
		m_dock = static_cast<Dock>(dock);
	}

	WNDCLASSW wndClass{};
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"Voicemeeter";
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = WndProcW;
	wndClass.hCursor = ::Windows::wLoadCursorW(NULL, IDC_ARROW);

	::Windows::wRegisterClassW(&wndClass);
	::Windows::wCreateWindowExW(
		WS_EX_NOREDIRECTIONBITMAP,
		wndClass.lpszClassName,
		NULL,
		WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		this
	);
}

void DeskBandit::Show(int nCmdShow) const {
	ShowWindow(
		m_hWnd,
		nCmdShow
	);
}

void DeskBandit::EnableInputTrack() {
	SetCapture(m_hWnd);
}
void DeskBandit::DisableInputTrack() {
	::Windows::wReleaseCapture();
}

LRESULT CALLBACK DeskBandit::WndProcW(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	auto shutdown = [uMsg](long long errCode)->LRESULT {
		::Windows::ErrorMessageBox(errCode);

		if (uMsg == WM_NCCREATE) {
			return FALSE;
		}
		PostQuitMessage(0);

		return OK;
	};
	auto log = [](const char* message)->void {
		WCHAR temp[MAX_PATH];
		if (GetTempPathW(MAX_PATH, temp)) {
			::std::wstring path{ temp };
			::std::fstream log{ path.append(L"Voicemeeter.DeskBand.log"), log.out | log.app };
			if (log.is_open()) {
				log << message << ::std::endl;
			}
		}
	};
	try {
		DeskBandit* pWnd{ ::Windows::wGetWindowLongPtrW<DeskBandit>(hWnd, GWLP_USERDATA) };
		switch (uMsg) {
		case WM_NCCREATE: {
			pWnd = reinterpret_cast<DeskBandit*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_hWnd = hWnd;
			::Windows::wSetParent(hWnd, pWnd->m_hWndParent);
			pWnd->m_pDockTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pDockTimer->Set(::std::chrono::milliseconds{ 100 },
				[pWnd]()->bool {
					switch (pWnd->m_dock) {
					case Dock::Right: {
						RECT tray{};
						::Windows::wGetWindowRect(pWnd->m_hWndTray, &tray);
						LONG diff{ tray.left - pWnd->m_rc.right };
						if (!diff) {
							return true;
						}
						pWnd->m_rc.right += diff;
						pWnd->m_rc.left += diff;
					} break;
					default:
						return false;
					}
					::Windows::wSetWindowPos(
						pWnd->m_hWnd, NULL,
						pWnd->m_rc.left, pWnd->m_rc.top,
						pWnd->m_rc.right - pWnd->m_rc.left, pWnd->m_rc.bottom - pWnd->m_rc.top,
						0U
					);
					return true;
				});
			pWnd->m_pCompositionTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pRenderTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pRemoteTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_lpTimer.emplace(pWnd->m_pDockTimer->get_Id(), pWnd->m_pDockTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pCompositionTimer->get_Id(), pWnd->m_pCompositionTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pRenderTimer->get_Id(), pWnd->m_pRenderTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pRemoteTimer->get_Id(), pWnd->m_pRemoteTimer.get());
			pWnd->m_pMixer.reset(new ::Voicemeeter::Adapters::Multiclient::Cherry{});
			pWnd->m_pRemote.reset(new ::Voicemeeter::Clients::Remote::Cherry{ *pWnd->m_pRemoteTimer, *pWnd->m_pMixer });
			bool d3d12{ true };
			if (d3d12) {
				pWnd->m_pRenderTimer->Set(::std::chrono::milliseconds{ USER_TIMER_MINIMUM },
					[pWnd]()->bool {
						pWnd->m_pD3d12Scene->Render();
						return true;
					});
				::Voicemeeter::Clients::UI::D3D12::Cherry builder{
					hWnd, NULL,
					*pWnd,
					*pWnd->m_pCompositionTimer,
					*pWnd->m_pMixer
				};
				if (pWnd->m_pRemote->get_Type() == ::Voicemeeter::Clients::Remote::Type::Voicemeeter) {
					builder
						.WithNetwork(false)
						.WithIgnoredStrip(3)
						.WithIgnoredStrip(5);
				}
				pWnd->m_pD3d12Scene = builder
					.WithTheme(::Voicemeeter::UI::Cherry::Graphics::Theme::Light())
					.WithMarginPosition({ 4., 4. })
					.WithMarginSize({ 4., 4. })
					.Build();
				pWnd->m_pScene = pWnd->m_pD3d12Scene.get();
			} else {
				pWnd->m_pRenderTimer->Set(::std::chrono::milliseconds{ USER_TIMER_MINIMUM },
					[pWnd]()->bool {
						pWnd->m_pD2dScene->Render();
						return true;
					});
				::Voicemeeter::Clients::UI::D2D::Cherry builder{
					hWnd,
					*pWnd,
					*pWnd->m_pCompositionTimer,
					*pWnd->m_pMixer
				};
				if (pWnd->m_pRemote->get_Type() == ::Voicemeeter::Clients::Remote::Type::Voicemeeter) {
					builder
						.WithNetwork(false)
						.WithIgnoredStrip(3)
						.WithIgnoredStrip(5);
				}
				pWnd->m_pD2dScene = builder
					.WithTheme(::Voicemeeter::UI::Cherry::Graphics::Theme::Light())
					.WithMarginPosition({ 4., 4. })
					.WithMarginSize({ 4., 4. })
					.Build();
				pWnd->m_pScene = pWnd->m_pD2dScene.get();
			}
			pWnd->m_pScene->Rescale({
				static_cast<double>(pWnd->m_rc.right - pWnd->m_rc.left),
				static_cast<double>(pWnd->m_rc.bottom - pWnd->m_rc.top)
			});
			const ::std::valarray<double>& vertex{ pWnd->m_pScene->get_Size() };
			pWnd->m_rc.right = pWnd->m_rc.left + static_cast<LONG>(::std::ceil(vertex[0]));
			::Windows::wSetWindowPos(
				pWnd->m_hWnd, NULL,
				pWnd->m_rc.left, pWnd->m_rc.top,
				pWnd->m_rc.right - pWnd->m_rc.left, pWnd->m_rc.bottom - pWnd->m_rc.top,
				0U
			);
			::Windows::wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		} break;
		case WM_DESTROY: {
			PostQuitMessage(0);
		} return OK;
		case WM_TIMER: {
			pWnd->m_lpTimer[static_cast<UINT_PTR>(wParam)]
				->Elapse();
		} return OK;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = ::Windows::wBeginPaint(hWnd, &ps);
			pWnd->m_pScene->Redraw(
				::std::valarray<double>{
					static_cast<double>(ps.rcPaint.left),
					static_cast<double>(ps.rcPaint.top)
				},
				(ps.rcPaint.right && ps.rcPaint.bottom
					? ::std::valarray<double>{
						static_cast<double>(ps.rcPaint.right),
						static_cast<double>(ps.rcPaint.bottom)
					} : pWnd->m_pScene->get_Size())
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
		}
	}
	catch (const ::Windows::Error& e) {
		log(e.what());

		return shutdown(e.code());
	}
	catch (const ::std::exception& e) {
		log(e.what());

		return shutdown(MSG_ERR_GENERAL);
	}
	catch (...) {
		return shutdown(MSG_ERR_GENERAL);
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}