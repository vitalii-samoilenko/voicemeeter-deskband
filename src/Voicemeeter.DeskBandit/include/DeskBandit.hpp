#ifndef DESKBANDIT_HPP
#define DESKBANDIT_HPP

#include <fstream>
#include <memory>
#include <string>

#include "wheel.hpp"

#include "Windows/API.hpp"
#include <windowsx.h>
#include "Windows/Timer.hpp"

#include "Voicemeeter/Cherry.hpp"
#include "Voicemeeter/Clients/Remote.hpp"
#include "Voicemeeter/Clients/UI/Canvas.hpp"
#include "Voicemeeter/Clients/UI/Composition.hpp"
#include "Voicemeeter/Clients/UI/FocusTracker.hpp"
#include "Voicemeeter/Clients/UI/Loader.hpp"
#include "Voicemeeter/Clients/UI/Palette.hpp"
#include "Voicemeeter/Clients/UI/Scene.hpp"
#include "Voicemeeter/Clients/UI/Theme.hpp"

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
		, _scene{ nullptr }
		, _dockTick{ nullptr } {
		::Windows::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		{
			DWORD dock{ 0UL };
			DWORD size{ sizeof(DWORD) };
			if (::Windows::RegGetValueW(
				HKEY_CURRENT_USER, LR"(SOFTWARE\VoicemeeterDeskBand)", L"Dock",
				RRF_RT_DWORD, NULL, &dock, &size) == ERROR_SUCCESS) {
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
			wndClass.hCursor = ::Windows::LoadCursorW(NULL, MAKEINTRESOURCEW(32512));
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
	using RemoteBuilder = ::Voicemeeter::Clients::RemoteBuilder<
		::Windows::Timer,
		::Voicemeeter::Cherry>;
	using SceneBuilder = ::Voicemeeter::Clients::UI::SceneBuilder<
		::Voicemeeter::Clients::UI::FocusTrackerBuilder<DeskBandit>,
		::Voicemeeter::Clients::UI::LoaderBuilder,
		::Voicemeeter::Clients::UI::PaletteBuilder,
		::Voicemeeter::Clients::UI::ThemeBuilder,
		::Voicemeeter::Clients::UI::CanvasBuilder<
			::Voicemeeter::Clients::UI::Loader,
			::Voicemeeter::Clients::UI::Palette,
			::Voicemeeter::Clients::UI::Theme,
			::Windows::Timer>,
		::Voicemeeter::Clients::UI::CompositionBuilder<
			::Windows::Timer,
			::Voicemeeter::Cherry,
			::Voicemeeter::Clients::UI::Canvas<
				::Voicemeeter::Clients::UI::Loader,
				::Voicemeeter::Clients::UI::Palette,
				::Voicemeeter::Clients::UI::Theme,
				::Windows::Timer>,
			::Voicemeeter::Clients::UI::FocusTracker<DeskBandit>>>;

	enum class Dock {
		Left = 0,
		MidLeft = 1,
		MidRight = 2,
		Right = 3
	};

	class DockTick final {
	public:
		inline DockTick(
			DeskBandit *that,
			::Windows::Timer &timer)
			: that{ that }
			, _timer{ timer } {

		};
		DockTick() = delete;
		DockTick(DockTick const &) = delete;
		DockTick(DockTick &&) = delete;

		inline ~DockTick() {
			Unset();
		};

		DockTick & operator=(DockTick const &) = delete;
		DockTick & operator=(DockTick &&) = delete;

		inline void operator()() {
			switch (that->_dock) {
			case Dock::Right: {
				RECT tray{};
				::Windows::GetWindowRect(that->_hWndTray, &tray);
				LONG diff{ tray.left - that->_rc.left - that->_rc.right };
				if (!diff) {
					return;
				}
				that->_rc.left += diff;
			} break;
			default: {
				Unset();
			} return;
			}
			::Windows::SetWindowPos(
				that->_hWnd, NULL,
				that->_rc.left, that->_rc.top,
				that->_rc.right, that->_rc.bottom,
				0U);
		};

		inline void Set() {
			_timer.Set(100, *this);
		};
		inline void Unset() {
			_timer.Unset(*this);
		};

	private:
		DeskBandit *that;
		::Windows::Timer &_timer;
	};

	friend class DockTick;

	HWND _hWndParent;
	HWND _hWndTray;
	RECT _rc;
	Dock _dock;
	HWND _hWnd;
	::std::unique_ptr<::Windows::Timer> _dockTimer;
	::std::unique_ptr<::Windows::Timer> _compositionTimer;
	::std::unique_ptr<::Windows::Timer> _renderTimer;
	::std::unique_ptr<::Windows::Timer> _remoteTimer;
	::std::unique_ptr<::Voicemeeter::Cherry> _mixer;
	::std::unique_ptr<RemoteBuilder::Remote> _remote;
	::std::unique_ptr<SceneBuilder::Scene> _scene;
	::std::unique_ptr<DockTick> _dockTick;

	inline static LRESULT CALLBACK WndProcW(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	) {
		constexpr LRESULT OK{ 0 };
		auto shutdown = [OK, uMsg](long long errCode)->LRESULT {
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
				::std::fstream log{
					path.append(L"Voicemeeter.DeskBand.log"),
					::std::ios::out | ::std::ios::app
				};
				if (log.is_open()) {
					log << message << ::std::endl;
				}
			}
		};
		try {
			DeskBandit *that{
				reinterpret_cast<DeskBandit *>(
					::Windows::GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			};
			switch (uMsg) {
			case WM_NCCREATE: {
				that = reinterpret_cast<DeskBandit *>(
					reinterpret_cast<LPCREATESTRUCTW>(lParam)
						->lpCreateParams);
				::Windows::SetWindowLongPtrW(hWnd, GWLP_USERDATA,
					reinterpret_cast<LONG_PTR>(that));
				that->_hWnd = hWnd;
				::Windows::SetParent(hWnd, that->_hWndParent);
				that->_dockTimer = ::std::make_unique<
					::Windows::Timer>(hWnd);
				that->_compositionTimer = ::std::make_unique<
					::Windows::Timer>(hWnd);
				that->_renderTimer = ::std::make_unique<
					::Windows::Timer>(hWnd);
				that->_remoteTimer = ::std::make_unique<
					::Windows::Timer>(hWnd);
				that->_mixer = ::std::make_unique<
					::Voicemeeter::Cherry>();
				{
					RemoteBuilder remoteBuilder{};
					remoteBuilder
						.set_Timer(*that->_remoteTimer)
						.set_Mixer(*that->_mixer);
					that->_remote = remoteBuilder.Build();
				}
				{
					SceneBuilder sceneBuilder{};
					sceneBuilder.get_FocusTrackerBuilder()
						.set_InputTracker(*that);
					sceneBuilder.get_ThemeBuilder()
						.set_Warning(vector_t{ push(215), push(215), push(215), push(255) })
						.set_Neutral(vector_t{ push(215), push(215), push(215), push(255) });
					sceneBuilder.get_CanvasBuilder()
						.set_hWnd(hWnd)
						.set_Timer(*that->_renderTimer);
					sceneBuilder.get_CompositionBuilder()
						.set_Timer(*that->_compositionTimer)
						.set_Mixer(*that->_mixer)
						.set_PaddingPosition(vector_t{ push(3), push(3) })
						.set_PaddingSize(vector_t{ push(3), push(3) });
					if (that->_remote->get_Type() == RemoteBuilder::Remote::Type::Voicemeeter) {
						sceneBuilder.get_CompositionBuilder()
							.set_Vban(false)
							.set_Enabled<::Voicemeeter::Cherry::Strips::A2>(false)
							.set_Enabled<::Voicemeeter::Cherry::Strips::B2>(false);
					}
					that->_scene = sceneBuilder.Build();
				}
				{
					RECT parent;
					::Windows::GetWindowRect(that->_hWndParent, &parent);
					that->_scene->Rescale(vector_t{
						push(parent.right - parent.left),
						push(parent.bottom - parent.top)
					});
					vector_t const &vertex{ that->_scene->get_Size() };
					that->_rc.right = static_cast<LONG>(pop(ceil(vertex[0])));
					that->_rc.bottom = static_cast<LONG>(pop(ceil(vertex[1])));
					::Windows::SetWindowPos(
						that->_hWnd, NULL,
						that->_rc.left, that->_rc.top,
						that->_rc.right, that->_rc.bottom,
						0U);
				}
				that->_dockTick = ::std::make_unique<
					DockTick>(that, *that->_dockTimer);
				that->_dockTick->Set();
			} break;
			case WM_DESTROY: {
				::PostQuitMessage(0);
			} return OK;
			case WM_SHOWWINDOW: {
				if (wParam == FALSE) {
					that->_scene->Hide();
				} else {
					that->_scene->Show();
				}
			} return OK;
			case WM_TIMER: {
				UINT_PTR id{ static_cast<UINT_PTR>(wParam) };
				::Windows::Timer *target{
					id == that->_renderTimer->get_Id()
						? that->_renderTimer.get()
						: id == that->_remoteTimer->get_Id()
							? that->_remoteTimer.get()
							: id == that->_dockTimer->get_Id()
								? that->_dockTimer.get()
								: id == that->_compositionTimer->get_Id()
									? that->_compositionTimer.get()
									: throw ::std::exception{ "Unknown timer" }
				};
				target->Elapse();
			} return OK;
			case WM_PAINT: {
				if (::GetUpdateRect(hWnd, NULL, FALSE)) {
					PAINTSTRUCT ps;
					HDC hdc{ ::Windows::BeginPaint(hWnd, &ps) };
					that->_scene->Redraw(
						vector_t{
							push(ps.rcPaint.left),
							push(ps.rcPaint.top)
						},
						vector_t{
							push(ps.rcPaint.right - ps.rcPaint.left),
							push(ps.rcPaint.bottom - ps.rcPaint.top)
						});
					::EndPaint(hWnd, &ps);
				}
			} return OK;
			case WM_LBUTTONDOWN: {
				that->_scene->MouseLDown(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_LBUTTONDBLCLK: {
				that->_scene->MouseLDouble(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_MBUTTONDOWN: {
				that->_scene->MouseMDown(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_MBUTTONDBLCLK: {
				that->_scene->MouseMDouble(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_RBUTTONDOWN: {
				that->_scene->MouseRDown(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_RBUTTONDBLCLK: {
				that->_scene->MouseRDouble(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_MOUSEWHEEL: {
				POINT point{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				::Windows::ScreenToClient(hWnd, &point);
				that->_scene->MouseWheel(vector_t{
					push(point.x),
					push(point.y)
				}, GET_WHEEL_DELTA_WPARAM(wParam));
			} return OK;
			case WM_MOUSEMOVE: {
				that->_scene->MouseMove(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_LBUTTONUP: {
				that->_scene->MouseLUp(vector_t{
					push(GET_X_LPARAM(lParam)),
					push(GET_Y_LPARAM(lParam))
				});
			} return OK;
			case WM_KEYDOWN: {
				if (LOWORD(wParam) == 'C' && ::GetKeyState(VK_CONTROL)) {
					::PostQuitMessage(0);
				}
			} return OK;
			}
		}
		catch (::Windows::Error const &e) {
			log(e.what());
			return shutdown(e.code());
		}
		catch (::std::exception const &e) {
			log(e.what());
			return shutdown(MSG_ERR_GENERAL);
		}
		catch (...) {
			return shutdown(MSG_ERR_GENERAL);
		}
		return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
	};
};

#endif
