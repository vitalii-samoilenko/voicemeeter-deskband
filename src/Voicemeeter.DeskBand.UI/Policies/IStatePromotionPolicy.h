#pragma once

#include "estd/type_traits.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState>
				class IStatePromotionPolicy {
				public:
					IStatePromotionPolicy() = delete;
					IStatePromotionPolicy(const IStatePromotionPolicy&) = delete;
					IStatePromotionPolicy(IStatePromotionPolicy&&) = delete;

					virtual ~IStatePromotionPolicy() = 0;

					IStatePromotionPolicy& operator=(const IStatePromotionPolicy&) = delete;
					IStatePromotionPolicy& operator=(IStatePromotionPolicy&&) = delete;

					const virtual void Promote(const estd::remove_cvref_t<TState>& state) = 0;
				};
			}
		}
	}
}