#pragma once

#include "Presentation/scene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Window {
			public:
				Window(HINSTANCE hInstance, Presentation::Scene& scene);
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
				Presentation::Scene& m_scene;

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