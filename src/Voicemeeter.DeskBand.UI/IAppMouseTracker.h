#pragma once

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IAppMouseTracker {
			public:
				IAppMouseTracker() = delete;
				IAppMouseTracker(const IAppMouseTracker&) = delete;
				IAppMouseTracker(IAppMouseTracker&&) = delete;

				~IAppMouseTracker() = delete;

				IAppMouseTracker& operator=(const IAppMouseTracker&) = delete;
				IAppMouseTracker& operator=(IAppMouseTracker&&) = delete;

				virtual void EnableMouseTrack() = 0;
				virtual void DisableMouseTrack() = 0;
			};
		}
	}
}