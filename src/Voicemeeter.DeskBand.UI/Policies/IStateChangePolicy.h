#pragma once

#include "estd/type_traits.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState>
				class IStateChangePolicy {
				public:
					IStateChangePolicy() = delete;
					IStateChangePolicy(const IStateChangePolicy&) = delete;
					IStateChangePolicy(IStateChangePolicy&&) = delete;

					virtual ~IStateChangePolicy() = 0;

					IStateChangePolicy& operator=(const IStateChangePolicy&) = delete;
					IStateChangePolicy& operator=(IStateChangePolicy&&) = delete;

					const virtual bool SetNext(estd::remove_cvref_t<TState>& state) = 0;
					const virtual bool SetPrevious(estd::remove_cvref_t<TState>& state) = 0;
					const virtual bool Set(estd::remove_cvref_t<TState>& to, TState&& from) = 0;
				};
			}
		}
	}
}