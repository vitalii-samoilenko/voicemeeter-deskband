#pragma once

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IMouseTracker {
			public:
				IMouseTracker(const IMouseTracker&) = delete;
				IMouseTracker(IMouseTracker&&) = delete;

				IMouseTracker& operator=(const IMouseTracker&) = delete;
				IMouseTracker& operator=(IMouseTracker&&) = delete;

				virtual void EnableMouseTrack(IComponent* pComponent) = 0;
				virtual void DisableMouseTrack(IComponent* pComponent) = 0;

			protected:
				IMouseTracker() = default;
				~IMouseTracker() = default;
			};
		}
	}
}