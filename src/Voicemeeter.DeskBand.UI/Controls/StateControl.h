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
						::std::is_base_of_v<IGlyph, ::estd::remove_cvref_t<TGlyph>>,
						"TGlyph must be derived from IGlyph");

				public:
					StateControl(
						const ::linear_algebra::vector& baseMarginTopLeft,
						const ::linear_algebra::vector& baseMarginBottomRight,
						::std::unique_ptr<::estd::remove_cvref_t<TGlyph>> pGlyph,
						::std::shared_ptr<IStateChangePolicy<::estd::remove_cvref_t<TState>>> pStateChangePolicy,
						::std::unique_ptr<IStatePromotionPolicy<::estd::remove_cvref_t<TState>>> pStatePromotionPolicy,
						::std::shared_ptr<IGlyphUpdatePolicy<::estd::remove_cvref_t<TGlyph>, ::estd::remove_cvref_t<TState>>> pGlyphUpdatePolicy,
						::std::shared_ptr<IInteractivityPolicy<StateControl>> pInteractivityPolicy
					) : Control{ baseMarginTopLeft, baseMarginBottomRight }
					  , m_state{}
					  , m_pGlyph{ ::std::move(pGlyph) }
					  , m_pStateChangePolicy{ ::std::move(pStateChangePolicy) }
					  , m_pStatePromotionPolicy{ ::std::move(pStatePromotionPolicy) }
					  , m_pGlyphUpdatePolicy{ ::std::move(pGlyphUpdatePolicy) }
					  , m_pInteractivityPolicy{ ::std::move(pInteractivityPolicy) } {
						m_pGlyph->Move(baseMarginTopLeft);
					};
					StateControl() = delete;
					StateControl(const StateControl&) = delete;
					StateControl(StateControl&&) = delete;

					~StateControl() = default;

					StateControl& operator=(const StateControl&) = delete;
					StateControl& operator=(StateControl&&) = delete;

					const ::estd::remove_cvref_t<TState>& get_State() const {
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

					virtual void MouseMove(const ::linear_algebra::vector& point) override {
						m_pInteractivityPolicy->MouseMove(*this, point);
					};
					virtual void MouseLUp(const ::linear_algebra::vector& point) override {
						m_pInteractivityPolicy->MouseLUp(*this, point);
					};

				protected:
					virtual const ::linear_algebra::vector& OnGet_Position() const override {
						return m_pGlyph->get_Position();
					};
					virtual const ::linear_algebra::vector& OnGet_Size() const override {
						return m_pGlyph->get_Size();
					};
					virtual const ::linear_algebra::vector& OnGet_BaseSize() const override {
						return m_pGlyph->get_BaseSize();
					};

					virtual void OnRedraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override {
						m_pGlyph->Redraw(point, vertex);
					};
					virtual void OnRescale(const ::linear_algebra::vector& vertex) override {
						m_pGlyph->Rescale(vertex);
					};
					virtual void OnMove(const ::linear_algebra::vector& point) override {
						m_pGlyph->Move(point);
					};
					virtual void OnMouseLDown(const ::linear_algebra::vector& point) override {
						m_pInteractivityPolicy->MouseLDown(*this, point);
					};
					virtual void OnMouseRDown(const ::linear_algebra::vector& point) override {
						m_pInteractivityPolicy->MouseRDown(*this, point);
					};
					virtual void OnMouseWheel(const ::linear_algebra::vector& point, int delta) override {
						m_pInteractivityPolicy->MouseWheel(*this, point, delta);
					};

				private:
					::estd::remove_cvref_t<TState> m_state;
					::std::unique_ptr<::estd::remove_cvref_t<TGlyph>> m_pGlyph;
					::std::shared_ptr<IStateChangePolicy<::estd::remove_cvref_t<TState>>> m_pStateChangePolicy;
					::std::unique_ptr<IStatePromotionPolicy<::estd::remove_cvref_t<TState>>> m_pStatePromotionPolicy;
					::std::shared_ptr<IGlyphUpdatePolicy<::estd::remove_cvref_t<TGlyph>, ::estd::remove_cvref_t<TState>>> m_pGlyphUpdatePolicy;
					::std::shared_ptr<IInteractivityPolicy<StateControl>> m_pInteractivityPolicy;

					void OnSet(bool promote) {
						if (promote) {
							m_pStatePromotionPolicy->Promote(m_state);
						}
						m_pGlyphUpdatePolicy->Update(*m_pGlyph, m_state);
						m_pGlyph->Redraw(::linear_algebra::Origin, ::linear_algebra::Infinity);
					}
				};
			}
		}
	}
}