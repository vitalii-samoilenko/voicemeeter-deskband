#pragma once

#include <windows.h>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Window {
			public:
				explicit Window(HINSTANCE hInstance);
				Window() = delete;
				Window(const Window&) = delete;
				Window(Window&&) = delete;

				~Window() = default;

				Window& operator=(const Window&) = delete;
				Window& operator=(Window&&) = delete;

				void Show(int nCmdShow) const;

			private:
				HWND m_hWnd;
				UINT m_dpi;

				static LRESULT WindowProcW(
					HWND hWnd,
					UINT uMsg,
					WPARAM wParam,
					LPARAM lParam
				);
			};
		}
	}
}