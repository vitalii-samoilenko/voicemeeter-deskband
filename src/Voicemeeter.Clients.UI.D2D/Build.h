#pragma once

#include "windows.h"

#include "Environment/IDirtyTracker.h"
#include "Environment/IInputTracker.h"
#include "Environment/ITimer.h"
#include "Voicemeeter.Clients.Remote/Client.h"
#include "Voicemeeter.UI.D2D/Scene.h"
#include "Voicemeeter.UI/Direction.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace D2D {
				template<typename TMixer>
				::Voicemeeter::UI::D2D::Scene* Build(
					HWND hWnd,
					::Voicemeeter::UI::Direction direction,
					::Environment::IDirtyTracker& dirtyTracker,
					::Environment::IInputTracker& inputTracker,
					::Environment::ITimer& compositionTimer,
					::Environment::ITimer& dirtyTimer,
					TMixer& mixer,
					::Voicemeeter::Clients::Remote::Client<TMixer>& remote
				);
			}
		}
	}
}