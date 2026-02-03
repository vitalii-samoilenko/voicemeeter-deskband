#ifndef DESKBANDIT_HPP
#define DESKBANDIT_HPP

#include <fstream>
#include <memory>
#include <string>

#include "Windows/API.hpp"
#include "Windows/Timer.hpp"
#include "Voicemeeter/Cherry.hpp"
#include "Voicemeeter/Clients/Remote.hpp"
#include "Voicemeeter/Clients/UI/Controls.hpp"
#include "Voicemeeter/Clients/UI/Graphics.hpp"
#include "Voicemeeter/Clients/UI/Palette.hpp"
#include "Voicemeeter/Clients/UI/Scene.hpp"
#include "Voicemeeter/Clients/UI/Theme.hpp"
#include "Voicemeeter/UI/Trackers/Focus.hpp"

#include "Messages.h"

class DeskBandit final {
public:
	inline explicit DeskBandit(HINSTANCE hInstance)
		: _hWndParent{ ::Windows::FindWindowExW(NULL, NULL, L"Shell_TrayWnd", NULL) }
		, _hWndTray{ ::Windows::FindWindowExW(_hWndParent, NULL, L"TrayNotifyWnd", NULL) }
		, _rc{}
		, _dock{ Dock::Left }
		, _hWnd{ NULL }
		, _dockTimer{ nullptr }
		, _compositionTimer{ nullptr }
		, _renderTimer{ nullptr }
		, _remoteTimer{ nullptr }
		, _mixer{ nullptr }
		, _remote{ nullptr }
		, _scene{ nullptr } {
		::Windows::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		{
			RECT parent{};
			::Windows::GetWindowRect(_hWndParent, &parent);
			_rc.bottom = parent.bottom - parent.top;
			_rc.right = parent.right - parent.left;
		}
		{
			DWORD dock{ 0UL };
			if (::Windows::RegGetValueW(
				HKEY_CURRENT_USER, LR"(SOFTWARE\VoicemeeterDeskBand)", L"Dock",
				RRF_RT_REG_DWORD, NULL, &dock, NULL) == ERROR_SUCCESS) {
				switch (dock) {
				case 0UL:
					_dock = Dock::Left;
					break;
				case 3UL:
					_dock = Dock::Right;
					break;
				}
			}
		}
		{
			WNDCLASSW wndClass{};
			wndClass.hInstance = hInstance;
			wndClass.lpszClassName = L"Voicemeeter";
			wndClass.style = CS_DBLCLKS;
			wndClass.lpfnWndProc = WndProcW;
			wndClass.hCursor = ::Windows::LoadCursorW(NULL, IDC_ARROW);
			::Windows::RegisterClassW(&wndClass);
			::Windows::CreateWindowExW(
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
	};
	DeskBandit() = delete;
	DeskBandit(DeskBandit const &) = delete;
	DeskBandit(DeskBandit &&) = delete;

	inline ~DeskBandit() = default;

	DeskBandit & operator=(DeskBandit const &) = delete;
	DeskBandit & operator=(DeskBandit &&) = delete;

	inline void Show(int nCmdShow) const {
		::ShowWindow(_hWnd, nCmdShow);
	};

	inline void EnableInputTrack() {
		::SetCapture(_hWnd);
	};
	inline void DisableInputTrack() {
		::Windows::ReleaseCapture();
	};

private:
	using Timer = ::Windows::Timer;
	using Mixer = ::Voicemeeter::Cherry;
	using Remote = ::Voicemeeter::Clients::Remote<Timer, Mixer>;
	using Palette = ::Voicemeeter::Clients::UI::Palette;
	using Theme = ::Voicemeeter::Clients::UI::Theme;
	using Canvas = ::Voicemeeter::Clients::UI::Graphics<Palette, Theme, Timer>;
	using FocusTracker = ::Voicemeeter::UI::Trackers::Focus;
	using Composition = ::Voicemeeter::Clients::UI::Controls<Timer, Mixer, Canvas, FocusTracker>;
	using Scene = ::Voicemeeter::Clients::UI::Scene<Canvas, Composition>;

	enum class Dock {
		Left = 0,
		MidLeft = 1,
		MidRight = 2,
		Right = 3
	};

	class DockTick final {
	public:
	private:
	};

	HWND _hWndParent;
	HWND _hWndTray;
	RECT _rc;
	Dock _dock;
	HWND _hWnd;
	::std::unique_ptr<Timer> _dockTimer;
	::std::unique_ptr<Timer> _compositionTimer;
	::std::unique_ptr<Timer> _renderTimer;
	::std::unique_ptr<Timer> _remoteTimer;
	::std::unique_ptr<Mixer> _mixer;
	::std::unique_ptr<Remote> _remote;
	::std::unique_ptr<Scene> _scene;

	inline static LRESULT CALLBACK WndProcW(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	) {
		constexpr LRESULT OK{ 0 };
		auto shutdown = [uMsg](long long errCode)->LRESULT {
			::Windows::ErrorMessageBox(NULL, CPT_ERROR, errCode);
			if (uMsg == WM_NCCREATE) {
				return FALSE;
			}
			::PostQuitMessage(0);
			return OK;
		};
		auto log = [](char const *message)->void {
			WCHAR temp[MAX_PATH];
			if (::GetTempPathW(MAX_PATH, temp)) {
				::std::wstring path{ temp };
				::std::fstream log{ path.append(L"Voicemeeter.DeskBand.log"), log.out | log.app };
				if (log.is_open()) {
					log << message << ::std::endl;
				}
			}
		};
		try {
			DeskBandit *wnd{
				reinterpret_cast<DeskBandit *>(
					::Windows::GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			};
			switch (uMsg) {
			case WM_NCCREATE: {
				wnd = reinterpret_cast<DeskBandit *>(
					reinterpret_cast<LPCREATESTRUCTW>(lParam)
						->lpCreateParams);
				wnd->_hWnd = hWnd;
				::Windows::SetParent(hWnd, wnd->_hWndParent);
				wnd->_dockTimer = ::std::make_unique<Timer>(hWnd);
				wnd->_dockTimer->Set(::std::chrono::milliseconds{ 100 },
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
				DWORD engine{ static_cast<DWORD>(RenderEngine::D3D12) };
				::Windows::Registry::TryGetValue(HKEY_CURRENT_USER, LR"(SOFTWARE\VoicemeeterDeskBand)", L"RenderEngine", engine);
				switch (static_cast<RenderEngine>(engine)) {
				default: {
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
					//builder
					//	.WithTheme(::Voicemeeter::UI::Cherry::Graphics::Theme::Light())
					//	.WithMarginPosition({ 4., 4. })
					//	.WithMarginSize({ 4., 4. });
					if (pWnd->m_pRemote->get_Type() == ::Voicemeeter::Clients::Remote::Type::Voicemeeter) {
						builder
							.WithNetwork(false)
							.WithIgnoredStrip(3)
							.WithIgnoredStrip(5);
					}
					pWnd->m_pD3d12Scene = builder
						.Build();
					pWnd->m_pScene = pWnd->m_pD3d12Scene.get();
				} break;
				}
				pWnd->m_pScene->Rescale({
					static_cast<double>(pWnd->m_rc.right - pWnd->m_rc.left),
					static_cast<double>(pWnd->m_rc.bottom - pWnd->m_rc.top)
				});
				const ::std::valarray<double>& vertex{ pWnd->m_pScene->get_Size() };
				pWnd->m_rc.right = pWnd->m_rc.left + static_cast<LONG>(::std::ceil(vertex[0]));
				//::Windows::wSetWindowPos(
				//	pWnd->m_hWnd, NULL,
				//	pWnd->m_rc.left, pWnd->m_rc.top,
				//	pWnd->m_rc.right - pWnd->m_rc.left, pWnd->m_rc.bottom - pWnd->m_rc.top,
				//	0U
				//);
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
	};
};

#endif
