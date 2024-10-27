#pragma once

namespace Voicemeeter {
	namespace UI {
		class ISystemInputTracker {
		public:				
			ISystemInputTracker(const ISystemInputTracker&) = delete;
			ISystemInputTracker(ISystemInputTracker&&) = delete;

			ISystemInputTracker& operator=(const ISystemInputTracker&) = delete;
			ISystemInputTracker& operator=(ISystemInputTracker&&) = delete;

			virtual void EnableInputTrack() = 0;
			virtual void DisableInputTrack() = 0;

		protected:
			ISystemInputTracker() = default;

			~ISystemInputTracker() = default;
		};
	}
}