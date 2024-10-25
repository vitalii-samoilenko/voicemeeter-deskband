#pragma once

#include <memory>

#include <windows.h>

#include "Voicemeeter.DeskBand.UI/IAppInputTracker.h"
#include "Voicemeeter.DeskBand.UI/IScene.h"

#include "../VoicemeeterRemote.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Window final : public ::Voicemeeter::DeskBand::UI::IAppInputTracker {
			public:
				explicit Window(
					HINSTANCE hInstance
				);
				Window() = delete;
				Window(const Window&) = delete;
				Window(Window&&) = delete;

				~Window();

				Window& operator=(const Window&) = delete;
				Window& operator=(Window&&) = delete;

				void Show(int nCmdShow) const;

				virtual void EnableInputTrack() override;
				virtual void DisableInputTrack() override;

			private:
				HWND m_hWnd;
				UINT m_dpi;
				::std::unique_ptr<::Voicemeeter::DeskBand::UI::IScene> m_pScene;
				T_VBVMR_INTERFACE remote;

				void BuildScene();
				void Connect();

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