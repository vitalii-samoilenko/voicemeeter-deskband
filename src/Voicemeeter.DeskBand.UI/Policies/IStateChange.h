#pragma once

#include "estd/type_traits.h"

namespace Voicemeeter {
	namespace DeskBand {
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

					virtual bool SetNext(::estd::remove_cvref_t<TState>& state) const = 0;
					virtual bool SetPrevious(::estd::remove_cvref_t<TState>& state) const = 0;
					virtual bool Set(::estd::remove_cvref_t<TState>& dst, ::estd::remove_cvref_t<TState>& src) const = 0;

				protected:
					IStateChange() = default;
				};
			}
		}
	}
}