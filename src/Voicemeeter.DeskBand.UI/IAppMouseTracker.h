#pragma once

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IAppMouseTracker {
			public:				
				IAppMouseTracker(const IAppMouseTracker&) = delete;
				IAppMouseTracker(IAppMouseTracker&&) = delete;

				IAppMouseTracker& operator=(const IAppMouseTracker&) = delete;
				IAppMouseTracker& operator=(IAppMouseTracker&&) = delete;

				virtual void EnableMouseTrack() = 0;
				virtual void DisableMouseTrack() = 0;

			protected:
				IAppMouseTracker() = default;

				~IAppMouseTracker() = default;
			};
		}
	}
}