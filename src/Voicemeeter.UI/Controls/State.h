#pragma once

#include <memory>
#include <utility>

#include "estd/type_traits.h"

#include "../Graphics/IGlyph.h"
#include "../IControl.h"
#include "../Trackers/IDirty.h"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<
				typename TState,
				typename TGlyph,
				typename TStateDefault,
				typename TStateNext,
				typename TStatePrevious,
				typename TStateSet,
				typename TChangeNotify,
				typename TGlyphUpdate,
				typename TScale>
			class State : public IControl {
				static_assert(
					::std::is_base_of_v<Graphics::IGlyph, TGlyph>(),
					"TGlyph must be derived from IGlyph");
				static_assert(
					::estd::is_invocable_r<bool, TStateDefault, TState&>(),
					"TStateDefault must be invocable with TState& arguments and bool return type");
				static_assert(
					::estd::is_invocable_r<bool, TStateNext, TState&>(),
					"TStateNext must be invocable with TState& arguments and bool return type");
				static_assert(
					::estd::is_invocable_r<bool, TStatePrevious, TState&>(),
					"TStatePrevious must be invocable with TState& arguments and bool return type");
				static_assert(
					::estd::is_invocable_r<bool, TStateSet, TState&, TState&>(),
					"TStateSet must be invocable with TState&, TState& arguments and bool return type");
				static_assert(
					::estd::is_invocable_r<void, TChangeNotify, const TState&>(),
					"TChangeNotify must be invocable with const TState& arguments and void return type");
				static_assert(
					::estd::is_invocable_r<void, TGlyphUpdate, TGlyph&>(),
					"TGlyphUpdate must be invocable with TGlyph& arguments and void return type");
				static_assert(
					::estd::is_invocable_r<::std::valarray<double>, TScale, const ::std::valarray<double>&, const ::std::valarray<double>&>(),
					"TScale must be invocable with const valarray<double>&, const valarray<double>& arguments and valarray<double> return type");

			public:
				State(
					const ::std::valarray<double>& baseVertex,
					Trackers::IDirty& dirtyTracker,
					::std::unique_ptr<TGlyph>& pGlyph,
					const TStateDefault& stateDefault = {},
					const TStateNext& stateNext = {},
					const TStatePrevious& statePrevious = {},
					const TStateSet& stateSet = {},
					const TChangeNotify& changeNotify = {},
					const TGlyphUpdate& glyphUpdate = {},
					const TScale& scale = {}
				) : m_point{ 0., 0. }
				  , m_vertex{ baseVertex }
				  , m_baseVertex{ baseVertex }
				  , m_state{}
				  , m_dirtyTracker{ dirtyTracker }
				  , m_pGlyph{ ::std::move(pGlyph) }
				  , m_stateDefault{ stateDefault }
				  , m_stateNext{ stateNext }
				  , m_statePrevious{ statePrevious }
				  , m_stateSet{ stateSet }
				  , m_changeNotify{ changeNotify }
				  , m_glyphUpdate{ glyphUpdate }
				  , m_scale{ scale } {
					if (m_stateDefault(m_state)) {
						m_glyphUpdate(*m_pGlyph, m_state);

						m_dirtyTracker.set_Dirty(*m_pGlyph, true);
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
					if (!m_stateDefault(m_state)) {
						return;
					}
					OnSet(true);
				}
				void SetNext() {
					if (!m_stateSet(m_state)) {
						return;
					}
					OnSet(true);
				};
				void SetPrevious() {
					if (!m_statePrevious(m_state)) {
						return;
					}
					OnSet(true);
				}
				void Set(TState& src, bool notify) {
					if (!m_stateSet(m_state, src)) {
						return;
					}
					OnSet(notify);
				}

				virtual void set_Focus(bool value) override {

				};
				virtual const ::std::valarray<double>& get_Position() const override {
					return m_point;
				};
				virtual const ::std::valarray<double>& get_Size() const override {
					return m_vertex;
				};
				virtual const ::std::valarray<double>& get_BaseSize() const override {
					return m_baseVertex;
				};

				virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
					m_pGlyph->Redraw(point, vertex);
				};
				virtual void Rescale(const ::std::valarray<double>& vertex) override {
					m_vertex = m_baseVertex * m_scale(m_baseVertex, vertex);

					m_pGlyph->Rescale(m_vertex);

					m_dirtyTracker.set_Dirty(*m_pGlyph, true);
				};
				virtual void Move(const ::std::valarray<double>& point) override {
					m_point = point;

					m_pGlyph->Move(point);

					m_dirtyTracker.set_Dirty(*m_pGlyph, true);
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
				::std::valarray<double> m_point;
				::std::valarray<double> m_vertex;
				const ::std::valarray<double> m_baseVertex;
				TState m_state;
				Trackers::IDirty& m_dirtyTracker;
				const ::std::unique_ptr<TGlyph> m_pGlyph;
				const TStateDefault m_stateDefault;
				const TStateNext m_stateNext;
				const TStatePrevious m_statePrevious;
				const TStateSet m_stateSet;
				const TChangeNotify m_changeNotify;
				const TGlyphUpdate m_glyphUpdate;
				const TScale m_scale;

				inline void OnSet(bool notify) const {
					if (notify) {
						m_changeNotify(m_state);
					}
					m_glyphUpdate(*m_pGlyph, m_state);

					m_dirtyTracker.set_Dirty(*m_pGlyph, true);
				}
			};
		}
	}
}