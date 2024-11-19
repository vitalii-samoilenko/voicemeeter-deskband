#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				template<typename TState>
				class IChange {
				public:
					IChange(const IChange&) = delete;
					IChange(IChange&&) = delete;

					virtual ~IChange() = default;

					IChange& operator=(const IChange&) = delete;
					IChange& operator=(IChange&&) = delete;

					virtual bool SetDefault(TState& state) const = 0;
					virtual bool SetNext(TState& state) const = 0;
					virtual bool SetPrevious(TState& state) const = 0;
					virtual bool Set(TState& dst, TState& src) const = 0;

				protected:
					IChange() = default;
				};
			}
		}
	}
}