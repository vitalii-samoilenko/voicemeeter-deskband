#pragma once

#include <memory>
#include <utility>

#include "estd/linear_algebra.h"
#include "estd/type_traits.h"

#include "../IControl.h"
#include "../Policies/IStateChangePolicy.h"
#include "../Policies/IStatePromotionPolicy.h"
#include "../Policies/IGlyphUpdatePolicy.h"
#include "../Policies/IInteractivityPolicy.h"

using namespace Voicemeeter::DeskBand::UI::Policies;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Controls {
				template<typename TState, typename TGlyph>
				class StateControl final : public IControl {
					static_assert(estd::is_base_of<IGlyph, TGlyph>(),
						"TGlyph must be derived from IGlyph");

				public:
					StateControl(
						std::unique_ptr<TGlyph> pGlyph,
						std::shared_ptr<IStateChangePolicy<TState>> pStateChangePolicy,
						std::unique_ptr<IStatePromotionPolicy<TState>> pStatePromotionPolicy,
						std::shared_ptr<IGlyphUpdatePolicy<TGlyph, TState>> pGlyphUpdatePolicy,
						std::shared_ptr<IInteractivityPolicy<StateControl>> pInteractivityPolicy
					) : m_pGlyph{ std::move(pGlyph) }
					  , m_pStateChangePolicy{ std::move(pStateChangePolicy) }
					  , m_pStatePromotionPolicy{ std::move(pStatePromotionPolicy) }
					  , m_pGlyphUpdatePolicy{ std::move(pGlyphUpdatePolicy) }
					  , m_pInteractivityPolicy{ std::move(pInteractivityPolicy) } {

					}
					StateControl() = delete;
					StateControl(const StateControl&) = delete;
					StateControl(StateControl&&) = delete;

					~StateControl() = default;

					StateControl& operator=(const StateControl&) = delete;
					StateControl& operator=(StateControl&&) = delete;

				private:
					std::unique_ptr<TGlyph> m_pGlyph;
					std::shared_ptr<IStateChangePolicy<TState>> m_pStateChangePolicy;
					std::unique_ptr<IStatePromotionPolicy<TState>> m_pStatePromotionPolicy;
					std::shared_ptr<IGlyphUpdatePolicy<TGlyph, TState>> m_pGlyphUpdatePolicy;
					std::shared_ptr<IInteractivityPolicy<StateControl>> m_pInteractivityPolicy;
				};
			}
		}
	}
}