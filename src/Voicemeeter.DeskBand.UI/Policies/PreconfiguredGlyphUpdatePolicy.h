#pragma once

#include "estd/type_traits.h"

#include "IGlyphUpdatePolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TGlyph, typename TState, typename Func>
				class PreconfiguredGlyphUpdatePolicy : public IGlyphUpdatePolicy<TGlyph, TState> {
					static_assert(estd::is_invocable<void, Func, estd::remove_cvref_t<TGlyph>&, const estd::remove_cvref_t<TState>&>(),
						"Func must be invocable with TGlyph and TState arguments and void return type");

				public:
					explicit PreconfiguredGlyphUpdatePolicy(
						Func onUpdate
					) : m_onUpdate{ onUpdate } {

					}
					PreconfiguredGlyphUpdatePolicy() = delete;
					PreconfiguredGlyphUpdatePolicy(const PreconfiguredGlyphUpdatePolicy&) = delete;
					PreconfiguredGlyphUpdatePolicy(PreconfiguredGlyphUpdatePolicy&&) = delete;

					~PreconfiguredGlyphUpdatePolicy() = default;

					PreconfiguredGlyphUpdatePolicy& operator=(const PreconfiguredGlyphUpdatePolicy&) = delete;
					PreconfiguredGlyphUpdatePolicy& operator=(PreconfiguredGlyphUpdatePolicy&&) = delete;

					const virtual void Update(estd::remove_cvref_t<TGlyph>& glyph, const estd::remove_cvref_t<TState>& state) override {
						m_onUpdate(glyph, state);
					};

				private:
					Func m_onUpdate;
				};
			}
		}
	}
}