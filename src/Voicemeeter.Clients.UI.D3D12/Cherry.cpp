#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX
#define _USE_MATH_DEFINES

#include "Cherry.h"

using namespace ::Voicemeeter::Clients::UI::D3D12;

::std::unique_ptr<::Voicemeeter::UI::D3D12::Scene> Cherry::Build() {
	LoadOverrides();
	::std::unique_ptr<::Voicemeeter::UI::D3D12::Graphics::Palette> pPalette{
		new ::Voicemeeter::UI::D3D12::Graphics::Palette{
			m_theme, m_direction == ::Voicemeeter::UI::Direction::Right,
			m_hWnd, m_hModule
		}
	};
	::std::unique_ptr<::Voicemeeter::UI::Trackers::Focus> pFocusTracker{
		new ::Voicemeeter::UI::Trackers::Focus{}
	};
	::std::unique_ptr<::Voicemeeter::UI::Trackers::Input> pInputTracker{
		new ::Voicemeeter::UI::Trackers::Input{ m_inputTracker }
	};
	::std::unique_ptr<::Voicemeeter::UI::Graphics::ICanvas> pCanvas{
		new ::Voicemeeter::UI::D3D12::Graphics::Canvas{ *pPalette }
	};
	::std::unique_ptr<::Voicemeeter::UI::IComponent> pComposition{
		Compose<::Voicemeeter::UI::D3D12::Scene>(
			*pPalette,
			*pFocusTracker, *pInputTracker
		)
	};
	return ::std::unique_ptr<::Voicemeeter::UI::D3D12::Scene>{
		new ::Voicemeeter::UI::D3D12::Scene{
			pPalette,
			pInputTracker, pFocusTracker,
			pCanvas, pComposition
		}
	};
}