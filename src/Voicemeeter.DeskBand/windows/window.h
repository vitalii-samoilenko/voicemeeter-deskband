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

				void Initialize();
				void Show(int nCmdShow);

			private:
				HINSTANCE m_hInstance;
				HWND m_hWnd;

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