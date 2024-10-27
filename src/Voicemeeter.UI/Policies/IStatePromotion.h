#pragma once

#include "estd/type_traits.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TState>
			class IStatePromotion {
			public:
				IStatePromotion(const IStatePromotion&) = delete;
				IStatePromotion(IStatePromotion&&) = delete;

				virtual ~IStatePromotion() = default;

				IStatePromotion& operator=(const IStatePromotion&) = delete;
				IStatePromotion& operator=(IStatePromotion&&) = delete;

				virtual void Promote(const ::estd::remove_cvref_t<TState>& state) const = 0;

			protected:
				IStatePromotion() = default;
			};
		}
	}
}