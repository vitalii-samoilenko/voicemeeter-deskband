#pragma once

#include <mutex>

#include <windows.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Window {
				LPCWSTR m_lpszClassName;
				HWND m_hWnd;
				HINSTANCE m_hInstance;

			protected:
				virtual LRESULT OnCreate();
				virtual LRESULT OnDestroy();
				virtual LRESULT OnPaint();
				virtual LRESULT OnSize(UINT w, UINT h);
				virtual LRESULT OnLButtonDown(LONG x, LONG y);

				Window(
					std::once_flag& rWndClassGuard,
					LPCWSTR lpszClassName,
					HINSTANCE hInstance
				);

				void Initialize(
					DWORD dwStyle,
					HWND hWndParent
				);
			public:
				static LRESULT CALLBACK WindowProcW(
					HWND hWnd,
					UINT uMsg,
					WPARAM wParam,
					LPARAM lParam
				);

				inline HWND get_hWnd() const noexcept {
					return m_hWnd;
				};
				inline HINSTANCE get_hInstance() const noexcept {
					return m_hInstance;
				};

				Window() = delete;
				Window(const Window&) = delete;
				Window(Window&&) = delete;

				virtual ~Window() = default;

				Window& operator=(const Window&) = delete;
				Window& operator=(Window&&) = delete;

				virtual void Initialize() = 0 { };
				virtual void Show(int nCmdShow);
			};
		}
	}
}