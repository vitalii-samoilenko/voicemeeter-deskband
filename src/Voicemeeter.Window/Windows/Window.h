#pragma once

#include <memory>

#include <windows.h>

#include "Environment/IInputTracker.h"
#include "Voicemeeter.Remote/Mixer.h"
#include "Voicemeeter.UI.D2D/Scene.h"
#include "Windows/Timer.h"

namespace Voicemeeter {
	namespace Windows {
		class Window final : public ::Environment::IInputTracker {
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

			virtual void EnableInputTrack() override;
			virtual void DisableInputTrack() override;

		private:
			HWND m_hWnd;
			UINT m_dpi;
			::std::unique_ptr<::Windows::Timer> m_pUiTimer;
			::std::unique_ptr<::Windows::Timer> m_pMixerTimer;
			::std::unique_ptr<::Voicemeeter::Remote::Mixer> m_pMixer;
			::std::unique_ptr<::Voicemeeter::UI::D2D::Scene> m_pScene;

			static LRESULT CALLBACK WndProcW(
				HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam
			);
		};
	}
}