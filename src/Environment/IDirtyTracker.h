#pragma once

namespace Environment {
	class IDirtyTracker {
	public:
		IDirtyTracker(const IDirtyTracker&) = delete;
		IDirtyTracker(IDirtyTracker&&) = delete;

		IDirtyTracker& operator=(const IDirtyTracker&) = delete;
		IDirtyTracker& operator=(IDirtyTracker&&) = delete;

		virtual void SetDirty() = 0;

	protected:
		IDirtyTracker() = default;

		~IDirtyTracker() = default;
	};
}