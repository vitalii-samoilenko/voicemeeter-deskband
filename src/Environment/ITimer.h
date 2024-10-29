#pragma once

#include <functional>
#include <chrono>

namespace Environment {
	class ITimer {
	public:
		ITimer(const ITimer&) = delete;
		ITimer(ITimer&&) = delete;

		ITimer& operator=(const ITimer&) = delete;
		ITimer& operator=(ITimer&&) = delete;

		virtual void Set(::std::chrono::milliseconds duration, const ::std::function<void()>& callback) = 0;
		virtual void Elapse() = 0;

	protected:
		ITimer() = default;

		~ITimer() = default;
	};
}