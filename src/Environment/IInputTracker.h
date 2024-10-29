#pragma once

namespace Environment {
	class IInputTracker {
	public:
		IInputTracker(const IInputTracker&) = delete;
		IInputTracker(IInputTracker&&) = delete;

		IInputTracker& operator=(const IInputTracker&) = delete;
		IInputTracker& operator=(IInputTracker&&) = delete;

		virtual void EnableInputTrack() = 0;
		virtual void DisableInputTrack() = 0;

	protected:
		IInputTracker() = default;

		~IInputTracker() = default;
	};
}