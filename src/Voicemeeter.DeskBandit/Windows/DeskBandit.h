#pragma once

#include <memory>
#include <unordered_map>

#include <windows.h>

#include "Environment/IDirtyTracker.h"
#include "Environment/IInputTracker.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"
#include "Voicemeeter.Clients.Remote/Cherry.h"
#include "Voicemeeter.Clients.UI.D2D/Cherry.h"
#include "Windows/Timer.h"

namespace Voicemeeter {
	namespace Windows {
		enum class Dock {
			Left = 0,
			MidLeft = 1,
			MidRight = 2,
			Right = 3
		};

		class DeskBandit final
			: public ::Environment::IDirtyTracker
			, public ::Environment::IInputTracker {
		public:
			explicit DeskBandit(
				HINSTANCE hInstance
			);
			DeskBandit() = delete;
			DeskBandit(const DeskBandit&) = delete;
			DeskBandit(DeskBandit&&) = delete;

			~DeskBandit() = default;

			DeskBandit& operator=(const DeskBandit&) = delete;
			DeskBandit& operator=(DeskBandit&&) = delete;

			void Show(int nCmdShow) const;

			virtual void SetDirty() override;
			virtual void EnableInputTrack() override;
			virtual void DisableInputTrack() override;

		private:
			HWND m_hWndParent;
			HWND m_hWndStart;
			HWND m_hWndBar;
			HWND m_hWndTray;
			Dock m_dock;
			HWND m_hWnd;
			UINT m_dpi;
			RECT m_rc;
			::std::unique_ptr<::Windows::Timer> m_pTrackTimer;
			::std::unique_ptr<::Windows::Timer> m_pCompositionTimer;
			::std::unique_ptr<::Windows::Timer> m_pDirtyTimer;
			::std::unique_ptr<::Windows::Timer> m_pRemoteTimer;
			::std::unordered_map<UINT_PTR, ::Windows::Timer*> m_lpTimer;
			::std::unique_ptr<::Voicemeeter::Adapters::Multiclient::Cherry> m_pMixer;
			::std::unique_ptr<::Voicemeeter::Clients::Remote::Cherry> m_pRemote;
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