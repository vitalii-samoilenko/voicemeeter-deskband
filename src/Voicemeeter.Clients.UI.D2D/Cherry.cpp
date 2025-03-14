#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX
#define _USE_MATH_DEFINES

#include "Cherry.h"

using namespace ::Voicemeeter::Clients::UI::D2D;

::std::unique_ptr<::Voicemeeter::UI::D2D::Scene> Cherry::Build() {
	LoadOverrides();
	::std::unique_ptr<::Voicemeeter::UI::D2D::Graphics::Palette> pPalette{
		new ::Voicemeeter::UI::D2D::Graphics::Palette{
			m_theme, m_direction == ::Voicemeeter::UI::Direction::Right,
			m_hWnd
		}
	};
	::std::unique_ptr<::Voicemeeter::UI::Trackers::Focus> pFocusTracker{
		new ::Voicemeeter::UI::Trackers::Focus{}
	};
	::std::unique_ptr<::Voicemeeter::UI::Trackers::Input> pInputTracker{
		new ::Voicemeeter::UI::Trackers::Input{ m_inputTracker }
	};
	::std::unique_ptr<::Voicemeeter::UI::Graphics::ICanvas> pCanvas{
		new ::Voicemeeter::UI::D2D::Graphics::Canvas{ *pPalette }
	};
	::std::unique_ptr<::Voicemeeter::UI::IComponent> pComposition{
		Compose<::Voicemeeter::UI::D2D::Scene>(
			*pPalette,
			*pFocusTracker, *pInputTracker
		)
	};
	return ::std::unique_ptr<::Voicemeeter::UI::D2D::Scene>{
		new ::Voicemeeter::UI::D2D::Scene{
			pPalette,
			pInputTracker, pFocusTracker,
			pCanvas, pComposition
		}
	};
}