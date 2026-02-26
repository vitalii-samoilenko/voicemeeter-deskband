#ifndef DESKBANDIT_HPP
#define DESKBANDIT_HPP

#include <fstream>
#include <memory>
#include <optional>
#include <string>

#include "wheel.hpp"

#include "Windows/API.hpp"
#include "Windows/Timer.hpp"

#include "Voicemeeter/Cherry.hpp"
#include "Voicemeeter/Clients/Remote.hpp"
#include "Voicemeeter/Clients/UI/Canvas.hpp"
#include "Voicemeeter/Clients/UI/Composition.hpp"
#include "Voicemeeter/Clients/UI/FocusTracker.hpp"
#include "Voicemeeter/Clients/UI/Loader.hpp"
#include "Voicemeeter/Clients/UI/Palette.hpp"
#include "Voicemeeter/Clients/UI/Scene.hpp"
#include "Voicemeeter/Clients/UI/Surface.hpp"
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
		, _surface{ nullptr }
		, _scene{ nullptr }
		, _dockTick{ nullptr } {
		::Windows::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
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
	using SurfaceBuilder = ::Voicemeeter::Clients::UI::SurfaceBuilder;
	using SceneBuilder = ::Voicemeeter::Clients::UI::SceneBuilder<
		::Voicemeeter::Clients::UI::FocusTrackerBuilder<DeskBandit>,
		::Voicemeeter::Clients::UI::LoaderBuilder,
		::Voicemeeter::Clients::UI::PaletteBuilder,
		::Voicemeeter::Clients::UI::ThemeBuilder,
		::Voicemeeter::Clients::UI::DirectCanvasBuilder<
			::Voicemeeter::Clients::UI::Surface,
			::Voicemeeter::Clients::UI::Loader,
			::Voicemeeter::Clients::UI::Palette,
			::Voicemeeter::Clients::UI::Theme,
			::Windows::Timer>,
		::Voicemeeter::Clients::UI::CompositionBuilder<
			::Windows::Timer,
			::Voicemeeter::Cherry,
			::Voicemeeter::Clients::UI::DirectCanvas<
				::Voicemeeter::Clients::UI::Surface,
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

	struct _config_Mixer {
		::std::optional<DWORD> Vban;
		::std::optional<DWORD> P;
		::std::optional<DWORD> V;
		::std::optional<DWORD> A1;
		::std::optional<DWORD> A2;
		::std::optional<DWORD> B1;
		::std::optional<DWORD> B2;
	};
	struct _config_Theme {
		::std::optional<DWORD> Inactive;
		::std::optional<DWORD> Active;
		::std::optional<DWORD> Warning;
		::std::optional<DWORD> Error;
		::std::optional<DWORD> Information;
		::std::optional<DWORD> Neutral;
		::std::optional<DWORD> EqLow;
		::std::optional<DWORD> EqMedium;
		::std::optional<DWORD> EqHigh;
	};
	struct _config_DeskBand {
		::std::optional<DWORD> Dock;
		_config_Mixer Mixer;
		_config_Theme Theme;
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
	::std::unique_ptr<SurfaceBuilder::Surface> _surface;
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
				_config_DeskBand config{};
				{
					auto loadValue = [](LPCWSTR lpSubKey, LPCWSTR lpValue, ::std::optional<DWORD> &target)->void {
						DWORD value{ 0UL };
						DWORD size{ sizeof(DWORD) };
						if (::Windows::RegGetValueW(
							HKEY_CURRENT_USER, lpSubKey, lpValue,
							RRF_RT_DWORD, NULL, &value, &size) == ERROR_SUCCESS) {
							target = value;
						}
					};
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand)", L"Dock", config.Dock);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"Vban", config.Mixer.Vban);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"P", config.Mixer.P);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"V", config.Mixer.V);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"A1", config.Mixer.A1);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"A2", config.Mixer.A2);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"B1", config.Mixer.B1);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Mixer)", L"B2", config.Mixer.B2);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"Inactive", config.Theme.Inactive);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"Active", config.Theme.Active);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"Warning", config.Theme.Warning);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"Error", config.Theme.Error);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"Information", config.Theme.Information);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"Neutral", config.Theme.Neutral);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"EqLow", config.Theme.EqLow);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"EqMedium", config.Theme.EqMedium);
					loadValue(LR"(SOFTWARE\VoicemeeterDeskBand\Theme)", L"EqHigh", config.Theme.EqHigh);
				}
				that = reinterpret_cast<DeskBandit *>(
					reinterpret_cast<LPCREATESTRUCTW>(lParam)
						->lpCreateParams);
				::Windows::SetWindowLongPtrW(hWnd, GWLP_USERDATA,
					reinterpret_cast<LONG_PTR>(that));
				that->_hWnd = hWnd;
				::Windows::SetParent(hWnd, that->_hWndParent);
				if (config.Dock) {
					switch (*(config.Dock)) {
					case 3:
						that->_dock = Dock::Right;
						break;
					}
				}
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
					SurfaceBuilder surfaceBuilder{};
					surfaceBuilder
						.set_hWnd(hWnd);
					that->_surface = surfaceBuilder.Build();
				}
				{
					auto toColor = [](DWORD rrggbbaa)->vector_t {
						WORD rrgg{ HIWORD(rrggbbaa) };
						WORD bbaa{ LOWORD(rrggbbaa) };
						return vector_t{
							push(HIBYTE(rrgg)),
							push(LOBYTE(rrgg)),
							push(HIBYTE(bbaa)),
							push(LOBYTE(bbaa))
						};
					};
					SceneBuilder sceneBuilder{};
					sceneBuilder.get_FocusTrackerBuilder()
						.set_InputTracker(*that);
					SceneBuilder::ThemeBuilder &themeBuilder{
						sceneBuilder.get_ThemeBuilder()
					};
					themeBuilder
						.set_Warning(vector_t{ push(215), push(215), push(215), push(255) })
						.set_Neutral(vector_t{ push(215), push(215), push(215), push(255) });
					if (config.Theme.Inactive) {
						themeBuilder.set_Inactive(toColor(
							*(config.Theme.Inactive)));
					}
					if (config.Theme.Active) {
						themeBuilder.set_Active(toColor(
							*(config.Theme.Active)));
					}
					if (config.Theme.Warning) {
						themeBuilder.set_Warning(toColor(
							*(config.Theme.Warning)));
					}
					if (config.Theme.Error) {
						themeBuilder.set_Error(toColor(
							*(config.Theme.Error)));
					}
					if (config.Theme.Information) {
						themeBuilder.set_Information(toColor(
							*(config.Theme.Information)));
					}
					if (config.Theme.Neutral) {
						themeBuilder.set_Neutral(toColor(
							*(config.Theme.Neutral)));
					}
					if (config.Theme.EqLow) {
						themeBuilder.set_EqLow(toColor(
							*(config.Theme.EqLow)));
					}
					if (config.Theme.EqMedium) {
						themeBuilder.set_EqMedium(toColor(
							*(config.Theme.EqMedium)));
					}
					if (config.Theme.EqHigh) {
						themeBuilder.set_EqHigh(toColor(
							*(config.Theme.EqHigh)));
					}
					sceneBuilder.get_CanvasBuilder()
						.set_Surface(*that->_surface)
						.set_Timer(*that->_renderTimer);
					SceneBuilder::CompositionBuilder &compositionBuilder{
						sceneBuilder.get_CompositionBuilder()
					};
					compositionBuilder
						.set_Timer(*that->_compositionTimer)
						.set_Mixer(*that->_mixer)
						.set_PaddingPosition(vector_t{ push(3), push(3) })
						.set_PaddingSize(vector_t{ push(3), push(3) });
					if (config.Mixer.Vban) {
						compositionBuilder.set_Vban(
							0 < *(config.Mixer.Vban));
					}
					if (config.Mixer.P) {
						compositionBuilder.set_Enabled<
							::Voicemeeter::Cherry::Strips::P>(
							0 < *(config.Mixer.P));
					}
					if (config.Mixer.V) {
						compositionBuilder.set_Enabled<
							::Voicemeeter::Cherry::Strips::V>(
							0 < *(config.Mixer.V));
					}
					if (config.Mixer.A1) {
						compositionBuilder.set_Enabled<
							::Voicemeeter::Cherry::Strips::A1>(
							0 < *(config.Mixer.A1));
					}
					if (config.Mixer.A2) {
						compositionBuilder.set_Enabled<
							::Voicemeeter::Cherry::Strips::A2>(
							0 < *(config.Mixer.A2));
					}
					if (config.Mixer.B1) {
						compositionBuilder.set_Enabled<
							::Voicemeeter::Cherry::Strips::B1>(
							0 < *(config.Mixer.B1));
					}
					if (config.Mixer.B2) {
						compositionBuilder.set_Enabled<
							::Voicemeeter::Cherry::Strips::B2>(
							0 < *(config.Mixer.B2));
					}
					if (that->_remote->get_Type() == RemoteBuilder::Remote::Type::Voicemeeter) {
						compositionBuilder
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
					/*
					// CachedCanvas only
					that->_surface->set_Size(vertex);
					*/
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
			/*
			// CachedCanvas only
			case WM_PAINT: {
				if (::GetUpdateRect(hWnd, NULL, FALSE)) {
					PAINTSTRUCT ps;
					HDC hdc{ ::Windows::BeginPaint(hWnd, &ps) };
					vector_t point{
						push(ps.rcPaint.left),
						push(ps.rcPaint.top)
					};
					vector_t vertex{
						push(ps.rcPaint.right - ps.rcPaint.left),
						push(ps.rcPaint.bottom - ps.rcPaint.top)
					};
					that->_surface->Prepare(point, vertex);
					that->_scene->Redraw(point, vertex);
					that->_surface->Commit();
					::EndPaint(hWnd, &ps);
				}
			} return OK;
			*/
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
					that->_remote = nullptr;
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
