#pragma once

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IMouseTracker {
			public:
				IMouseTracker() = delete;
				IMouseTracker(const IMouseTracker&) = delete;
				IMouseTracker(IMouseTracker&&) = delete;

				~IMouseTracker() = delete;

				IMouseTracker& operator=(const IMouseTracker&) = delete;
				IMouseTracker& operator=(IMouseTracker&&) = delete;

				virtual void EnableMouseTrack(IComponent& component) = 0;
				virtual void DisableMouseTrack(IComponent& component) = 0;
			};
		}
	}
}