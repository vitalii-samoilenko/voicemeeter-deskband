#pragma once

#include <memory>
#include <utility>

#include "estd/linear_algebra.h"
#include "estd/type_traits.h"

#include "../IControl.h"
#include "../Policies/IStateChange.h"
#include "../Policies/IStatePromotion.h"
#include "../Policies/IGlyphUpdate.h"
#include "../Policies/IInteractivity.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Controls {
				template<typename TState, typename TGlyph>
				class State : public IControl {
				public:
					State(
						::std::unique_ptr<TGlyph> pGlyph,
						::std::shared_ptr<Policies::IStateChange<TState>> pStateChangePolicy,
						::std::unique_ptr<Policies::IStatePromotion<TState>> pStatePromotionPolicy,
						::std::shared_ptr<Policies::IGlyphUpdate<TGlyph, TState>> pGlyphUpdatePolicy,
						::std::shared_ptr<Policies::IInteractivity<State>> pInteractivityPolicy
					) : m_state{}
					  , m_pGlyph{ ::std::move(pGlyph) }
					  , m_pStateChangePolicy{ ::std::move(pStateChangePolicy) }
					  , m_pStatePromotionPolicy{ ::std::move(pStatePromotionPolicy) }
					  , m_pGlyphUpdatePolicy{ ::std::move(pGlyphUpdatePolicy) }
					  , m_pInteractivityPolicy{ ::std::move(pInteractivityPolicy) } {

					};
					State() = delete;
					State(const State&) = delete;
					State(State&&) = delete;

					~State() = default;

					State& operator=(const State&) = delete;
					State& operator=(State&&) = delete;

					const ::estd::remove_cvref_t<TState>& get_State() const {
						return m_state;
					};

					void SetDefault() {
						if (!m_pStateChangePolicy->SetDefault(m_state)) {
							return;
						}

						OnSet(true);
					}
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

					virtual const ::linear_algebra::vectord& get_Position() const override {
						return m_pGlyph->get_Position();
					};
					virtual const ::linear_algebra::vectord& get_Size() const override {
						return m_pGlyph->get_Size();
					};
					virtual const ::linear_algebra::vectord& get_BaseSize() const override {
						return m_pGlyph->get_BaseSize();
					};

					virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
						m_pGlyph->Redraw(point, vertex);
					};
					virtual void Rescale(const ::linear_algebra::vectord& vertex) override {
						m_pGlyph->Rescale(vertex);
					};
					virtual void Move(const ::linear_algebra::vectord& point) override {
						m_pGlyph->Move(point);
					};
					virtual bool MouseLDown(const ::linear_algebra::vectord& point) override {
						m_pInteractivityPolicy->MouseLDown(*this, point);

						return true;
					};
					virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override {
						m_pInteractivityPolicy->MouseLDouble(*this, point);

						return true;
					};
					virtual bool MouseRDown(const ::linear_algebra::vectord& point) override {
						m_pInteractivityPolicy->MouseRDown(*this, point);

						return true;
					};
					virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override {
						m_pInteractivityPolicy->MouseWheel(*this, point, delta);

						return true;
					};
					virtual bool MouseMove(const ::linear_algebra::vectord& point) override {
						m_pInteractivityPolicy->MouseMove(*this, point);

						return true;
					};
					virtual bool MouseLUp(const ::linear_algebra::vectord& point) override {
						m_pInteractivityPolicy->MouseLUp(*this, point);

						return true;
					};

				private:
					::estd::remove_cvref_t<TState> m_state;
					::std::unique_ptr<::estd::remove_cvref_t<TGlyph>> m_pGlyph;
					::std::shared_ptr<Policies::IStateChange<::estd::remove_cvref_t<TState>>> m_pStateChangePolicy;
					::std::unique_ptr<Policies::IStatePromotion<::estd::remove_cvref_t<TState>>> m_pStatePromotionPolicy;
					::std::shared_ptr<Policies::IGlyphUpdate<::estd::remove_cvref_t<TGlyph>, ::estd::remove_cvref_t<TState>>> m_pGlyphUpdatePolicy;
					::std::shared_ptr<Policies::IInteractivity<State>> m_pInteractivityPolicy;

					void OnSet(bool promote) {
						if (promote) {
							m_pStatePromotionPolicy->Promote(m_state);
						}
						m_pGlyphUpdatePolicy->Update(*m_pGlyph, m_state);
						m_pGlyph->Redraw(::linear_algebra::vectord::origin(), ::linear_algebra::vectord::infinity());
					}
				};
			}
		}
	}
}