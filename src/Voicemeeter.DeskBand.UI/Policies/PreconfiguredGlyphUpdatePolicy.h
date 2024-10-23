#pragma once

#include <utility>

#include "estd/type_traits.h"

#include "IGlyphUpdatePolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TGlyph, typename TState, typename TFunc>
				class PreconfiguredGlyphUpdatePolicy final : public IGlyphUpdatePolicy<TGlyph, TState> {
					static_assert(
						::estd::is_invocable_r<void, ::estd::remove_cvref_t<TFunc>, ::estd::remove_cvref_t<TGlyph>&, const ::estd::remove_cvref_t<TState>&>(),
						"TFunc must be invocable with TGlyph& and const TState& arguments and void return type");

				public:
					explicit PreconfiguredGlyphUpdatePolicy(
						::estd::remove_cvref_t<TFunc>& onUpdate
					) : m_onUpdate{ ::std::move(onUpdate) } {

					}
					PreconfiguredGlyphUpdatePolicy() = delete;
					PreconfiguredGlyphUpdatePolicy(const PreconfiguredGlyphUpdatePolicy&) = delete;
					PreconfiguredGlyphUpdatePolicy(PreconfiguredGlyphUpdatePolicy&&) = delete;

					~PreconfiguredGlyphUpdatePolicy() = default;

					PreconfiguredGlyphUpdatePolicy& operator=(const PreconfiguredGlyphUpdatePolicy&) = delete;
					PreconfiguredGlyphUpdatePolicy& operator=(PreconfiguredGlyphUpdatePolicy&&) = delete;

					virtual void Update(::estd::remove_cvref_t<TGlyph>& glyph, const ::estd::remove_cvref_t<TState>& state) const override {
						m_onUpdate(glyph, state);
					};

				private:
					::estd::remove_cvref_t<TFunc> m_onUpdate;
				};
			}
		}
	}
}