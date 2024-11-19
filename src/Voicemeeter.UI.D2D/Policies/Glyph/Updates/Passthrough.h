#pragma once

#include "Voicemeeter.UI/Policies/Glyph/IUpdate.h"

#include "../../../Adapters/Glyph/IUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace Glyph {
					namespace Updates {
						template<typename TGlyph, typename TState>
						class Passthrough : public UI::Policies::Glyph::IUpdate<Adapters::Glyph::IUpdate<TGlyph, TState>, TState> {
						public:
							Passthrough() = default;
							Passthrough(const Passthrough&) = delete;
							Passthrough(Passthrough&&) = delete;

							~Passthrough() = default;

							Passthrough& operator=(const Passthrough&) = delete;
							Passthrough& operator=(Passthrough&&) = delete;

							virtual void Update(Adapters::Glyph::IUpdate<TGlyph, TState>& glyph, const TState& state) const override {
								glyph.Update(state);
							}
						};
					}
				}
			}
		}
	}
}