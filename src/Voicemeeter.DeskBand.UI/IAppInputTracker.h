#pragma once

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IAppInputTracker {
			public:				
				IAppInputTracker(const IAppInputTracker&) = delete;
				IAppInputTracker(IAppInputTracker&&) = delete;

				IAppInputTracker& operator=(const IAppInputTracker&) = delete;
				IAppInputTracker& operator=(IAppInputTracker&&) = delete;

				virtual void EnableInputTrack() = 0;
				virtual void DisableInputTrack() = 0;

			protected:
				IAppInputTracker() = default;

				~IAppInputTracker() = default;
			};
		}
	}
}