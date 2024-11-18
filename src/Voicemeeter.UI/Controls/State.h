#pragma once

#include <memory>
#include <utility>

#include "../Policies/IStateChange.h"
#include "../Policies/IStatePromotion.h"
#include "../Policies/IGlyphUpdate.h"
#include "../IControl.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<typename TState, typename TGlyph>
			class State : public IControl {
			public:
				State(
					::std::unique_ptr<TGlyph>& pGlyph,
					const ::std::shared_ptr<Policies::IStateChange<TState>>& pStateChangePolicy,
					::std::unique_ptr<Policies::IStatePromotion<TState>>& pStatePromotionPolicy,
					const ::std::shared_ptr<Policies::IGlyphUpdate<TGlyph, TState>>& pGlyphUpdatePolicy
				) : m_state{}
				  , m_pGlyph{ ::std::move(pGlyph) }
				  , m_pStateChangePolicy{ pStateChangePolicy }
				  , m_pStatePromotionPolicy{ ::std::move(pStatePromotionPolicy) }
				  , m_pGlyphUpdatePolicy{ pGlyphUpdatePolicy } {
					if (m_pStateChangePolicy->SetDefault(m_state)) {
						m_pGlyphUpdatePolicy->Update(*m_pGlyph, m_state);
					}
				};
				State() = delete;
				State(const State&) = delete;
				State(State&&) = delete;

				~State() = default;

				State& operator=(const State&) = delete;
				State& operator=(State&&) = delete;

				const TState& get_State() const {
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
				void Set(TState& src, bool promote) {
					if (!m_pStateChangePolicy->Set(m_state, src)) {
						return;
					}

					OnSet(promote);
				}

				virtual void set_Focus(bool value) override {

				};
				virtual const ::std::valarray<double>& get_Position() const override {
					return m_pGlyph->get_Position();
				};
				virtual const ::std::valarray<double>& get_Size() const override {
					return m_pGlyph->get_Size();
				};
				virtual const ::std::valarray<double>& get_BaseSize() const override {
					return m_pGlyph->get_BaseSize();
				};

				virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
					m_pGlyph->Redraw(point, vertex);
				};
				virtual void Rescale(const ::std::valarray<double>& vertex) override {
					m_pGlyph->Rescale(vertex);
				};
				virtual void Move(const ::std::valarray<double>& point) override {
					m_pGlyph->Move(point);
				};
				virtual bool MouseLDown(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseMDown(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseMDouble(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseRDown(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseRDouble(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
					return true;
				};
				virtual bool MouseMove(const ::std::valarray<double>& point) override {
					return true;
				};
				virtual bool MouseLUp(const ::std::valarray<double>& point) override {
					return true;
				};

			private:
				TState m_state;
				::std::unique_ptr<TGlyph> m_pGlyph;
				::std::shared_ptr<Policies::IStateChange<TState>> m_pStateChangePolicy;
				::std::unique_ptr<Policies::IStatePromotion<TState>> m_pStatePromotionPolicy;
				::std::shared_ptr<Policies::IGlyphUpdate<TGlyph, TState>> m_pGlyphUpdatePolicy;

				void OnSet(bool promote) {
					if (promote) {
						m_pStatePromotionPolicy->Promote(m_state);
					}
					m_pGlyphUpdatePolicy->Update(*m_pGlyph, m_state);
				}
			};
		}
	}
}