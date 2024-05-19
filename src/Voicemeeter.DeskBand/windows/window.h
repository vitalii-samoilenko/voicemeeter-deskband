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

				void Show(int nCmdShow) const;

			private:
				HWND m_hWnd;
				Presentation::Scene& m_scene;
				ComPtr<IDCompositionTarget> m_pCompTarget;

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