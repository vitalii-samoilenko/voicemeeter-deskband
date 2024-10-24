#pragma once

#include <memory>

#include <windows.h>

#include "Voicemeeter.DeskBand.UI/IAppMouseTracker.h"
#include "Voicemeeter.DeskBand.UI/IScene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Window final : public ::Voicemeeter::DeskBand::UI::IAppMouseTracker {
			public:
				explicit Window(
					HINSTANCE hInstance
				);
				Window() = delete;
				Window(const Window&) = delete;
				Window(Window&&) = delete;

				~Window() = default;

				Window& operator=(const Window&) = delete;
				Window& operator=(Window&&) = delete;

				void Show(int nCmdShow) const;

				virtual void EnableMouseTrack() override;
				virtual void DisableMouseTrack() override;

			private:
				HWND m_hWnd;
				UINT m_dpi;
				::std::unique_ptr<::Voicemeeter::DeskBand::UI::IScene> m_pScene;

				void BuildScene();

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