#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				template<typename TState>
				class IPromotion {
				public:
					IPromotion(const IPromotion&) = delete;
					IPromotion(IPromotion&&) = delete;

					virtual ~IPromotion() = default;

					IPromotion& operator=(const IPromotion&) = delete;
					IPromotion& operator=(IPromotion&&) = delete;

					virtual void Promote(const TState& state) const = 0;

				protected:
					IPromotion() = default;
				};
			}
		}
	}
}