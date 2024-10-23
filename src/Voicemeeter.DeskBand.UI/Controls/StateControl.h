#pragma once

#include <memory>
#include <utility>

#include "estd/linear_algebra.h"

#include "../Control.h"
#include "../Policies/IStateChangePolicy.h"
#include "../Policies/IStatePromotionPolicy.h"
#include "../Policies/IGlyphUpdatePolicy.h"
#include "../Policies/IInteractivityPolicy.h"

using namespace ::Voicemeeter::DeskBand::UI::Policies;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Controls {
				template<typename TState, typename TGlyph>
				class StateControl final : public Control {
					static_assert(
						::std::is_base_of_v<IGlyph, TGlyph>,
						"TGlyph must be derived from IGlyph");

				public:
					StateControl(
						::std::unique_ptr<TGlyph> pGlyph,
						::std::shared_ptr<IStateChangePolicy<TState>> pStateChangePolicy,
						::std::unique_ptr<IStatePromotionPolicy<TState>> pStatePromotionPolicy,
						::std::shared_ptr<IGlyphUpdatePolicy<TGlyph, TState>> pGlyphUpdatePolicy,
						::std::shared_ptr<IInteractivityPolicy<StateControl>> pInteractivityPolicy
					) : m_state{}
					  , m_pGlyph{ ::std::move(pGlyph) }
					  , m_pStateChangePolicy{ ::std::move(pStateChangePolicy) }
					  , m_pStatePromotionPolicy{ ::std::move(pStatePromotionPolicy) }
					  , m_pGlyphUpdatePolicy{ ::std::move(pGlyphUpdatePolicy) }
					  , m_pInteractivityPolicy{ ::std::move(pInteractivityPolicy) } {
						m_baseVertex = m_pGlyph->get_baseSize();
					};
					StateControl() = delete;
					StateControl(const StateControl&) = delete;
					StateControl(StateControl&&) = delete;

					~StateControl() = default;

					StateControl& operator=(const StateControl&) = delete;
					StateControl& operator=(StateControl&&) = delete;

					::estd::remove_cvref_t<TState> get_State() const {
						return m_state;
					};

					void SetNext() {
						if (!m_pStateChangePolicy->SetNext(m_state)) {
							return;
						}

						OnSet(true);
					};
					void SetPrevious() {
						if (!m_pStateChangePolicy->SetPrevious(m_state)) {
							return;
						}

						OnSet(true);
					}
					void Set(::estd::remove_cvref_t<TState>& src, bool promote) {
						if (!m_pStateChangePolicy->Set(m_state, src)) {
							return;
						}

						OnSet(promote);
					}
					virtual void MouseMove(linear_algebra::vector point) override {
						m_pInteractivityPolicy->MouseMove(this, point);
					};
					virtual void MouseLUp(linear_algebra::vector point) override {
						m_pInteractivityPolicy->MouseLUp(this, point);
					};

				protected:
					virtual void OnRedraw(linear_algebra::vector point, linear_algebra::vector vertex) override {
						m_pGlyph->Redraw(point, vertex);
					};
					virtual void OnRescale(linear_algebra::vector vertex) override {
						m_pGlyph->Rescale(vertex);

						m_vertex = m_pGlyph->get_Size();
					};
					virtual void OnMove(linear_algebra::vector point) override {
						m_pGlyph->Move(point);
					};
					virtual void OnMouseLDown(linear_algebra::vector point) override {
						m_pInteractivityPolicy->MouseLDown(this, point);
					};
					virtual void OnMouseRDown(linear_algebra::vector point) override {
						m_pInteractivityPolicy->MouseRDown(this, point);
					};
					virtual void OnMouseWheel(linear_algebra::vector point, int delta) override {
						m_pInteractivityPolicy->MouseWheel(this, point, delta);
					};

				private:
					TState m_state;
					::std::unique_ptr<TGlyph> m_pGlyph;
					::std::shared_ptr<IStateChangePolicy<TState>> m_pStateChangePolicy;
					::std::unique_ptr<IStatePromotionPolicy<TState>> m_pStatePromotionPolicy;
					::std::shared_ptr<IGlyphUpdatePolicy<TGlyph, TState>> m_pGlyphUpdatePolicy;
					::std::shared_ptr<IInteractivityPolicy<StateControl>> m_pInteractivityPolicy;

					void OnSet(bool promote) {
						if (promote) {
							m_pStatePromotionPolicy->Promote(m_state);
						}
						m_pGlyphUpdatePolicy->Update(*m_pGlyph, m_state);
						m_pGlyph->Redraw(m_point, m_vertex);
					}
				};
			}
		}
	}
}