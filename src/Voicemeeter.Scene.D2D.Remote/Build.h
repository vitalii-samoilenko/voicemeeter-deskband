#pragma once

#include "windows.h"

#include "Environment/IDirtyTracker.h"
#include "Environment/IInputTracker.h"
#include "Environment/ITimer.h"
#include "Voicemeeter.Remote/Mixer.h"
#include "Voicemeeter.UI.D2D/Scene.h"
#include "Voicemeeter.UI/Direction.h"

namespace Voicemeeter {
	namespace Scene {
		namespace D2D {
			namespace Remote {
				UI::D2D::Scene* Build(
					HWND hWnd,
					UI::Direction direction,
					::Environment::IDirtyTracker& dirtyTracker,
					::Environment::IInputTracker& inputTracker,
					::Environment::ITimer& compositionTimer,
					::Environment::ITimer& dirtyTimer,
					::Voicemeeter::Remote::Mixer& mixer
				);
			}
		}
	}
}