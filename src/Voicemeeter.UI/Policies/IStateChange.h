#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TState>
			class IStateChange {
			public:
				IStateChange(const IStateChange&) = delete;
				IStateChange(IStateChange&&) = delete;

				virtual ~IStateChange() = default;

				IStateChange& operator=(const IStateChange&) = delete;
				IStateChange& operator=(IStateChange&&) = delete;

				virtual bool SetDefault(TState& state) const = 0;
				virtual bool SetNext(TState& state) const = 0;
				virtual bool SetPrevious(TState& state) const = 0;
				virtual bool Set(TState& dst, TState& src) const = 0;

			protected:
				IStateChange() = default;
			};
		}
	}
}