#pragma once

#include "estd/type_traits.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState>
				class IStatePromotionPolicy {
				public:
					IStatePromotionPolicy(const IStatePromotionPolicy&) = delete;
					IStatePromotionPolicy(IStatePromotionPolicy&&) = delete;

					virtual ~IStatePromotionPolicy() = default;

					IStatePromotionPolicy& operator=(const IStatePromotionPolicy&) = delete;
					IStatePromotionPolicy& operator=(IStatePromotionPolicy&&) = delete;

					virtual void Promote(const ::estd::remove_cvref_t<TState>& state) const = 0;

				protected:
					IStatePromotionPolicy() = default;
				};
			}
		}
	}
}