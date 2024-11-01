#pragma once

#include "windows.h"

#include "Environment/IInputTracker.h"
#include "Environment/ITimer.h"
#include "Voicemeeter.Remote/Mixer.h"
#include "Voicemeeter.UI.D2D/Scene.h"

namespace Voicemeeter {
	namespace Scene {
		namespace D2D {
			namespace Remote {
				UI::D2D::Scene* Build(
					HWND hWnd,
					::Environment::IInputTracker& inputTracker,
					::Environment::ITimer& timer,
					::Voicemeeter::Remote::Mixer& mixer
				);
			}
		}
	}
}