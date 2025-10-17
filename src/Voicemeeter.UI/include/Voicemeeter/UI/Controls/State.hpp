#ifndef VOICEMEETER_UI_CONTROLS_STATE_HPP
#define VOICEMEETER_UI_CONTROLS_STATE_HPP

#include <memory>
#include <utility>
#include <valarray>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Controls {
			template<
				typename TState,
				typename TGlyph,
				typename TChangeNotify,
				typename TStateChange,
				typename TGlyphUpdate,
				typename TScale>
			class State {
			public:
				inline State(
					::std::valarray<double> &&baseVertex,
					::std::unique_ptr<TGlyph> &&glyph,
					TChangeNotify &&changeNotify,
					TStateChange &&stateChange = TStateChange{},
					TGlyphUpdate &&glyphUpdate = TGlyphUpdate{},
					TScale &&scale = TScale{})
					: _point{ ::std::valarray<double>(0, baseVertex.size()) }
					, _vertex{ baseVertex }
					, _baseVertex{ ::std::move(baseVertex) }
					, _state{}
					, _glyph{ ::std::move(pGlyph) }
					, _changeNotify{ changeNotify }
					, _stateChange{ ::std::move(stateChange) }
					, _glyphUpdate{ ::std::move(glyphUpdate) }
					, _scale{ ::std::move(scale) } {
					if (!_stateDefault(_state)) {
						return;
					}
					OnSet(false);
				};
				State() = delete;
				State(State const &) = delete;
				State(State &&) = delete;

				inline ~State() = default;

				State & operator=(State const &) = delete;
				State & operator=(State &&) = delete;

				inline TState const & get_State() const {
					return _state;
				};

				inline void SetDefault() {
					if (!_state.SetDefault(_state)) {
						return;
					}
					OnSet(true);
				};
				inline void SetNext() {
					if (!_state.SetNext(_state)) {
						return;
					}
					OnSet(true);
				};
				inline void SetPrevious() {
					if (!_state.SetPrevious(_state)) {
						return;
					}
					OnSet(true);
				};
				inline void Set(TState &dst, bool notify = true) {
					if (!_state.Set(_state, dst)) {
						return;
					}
					OnSet(notify);
				};

				inline ::std::valarray<double> const & get_Position() const {
					return _point;
				};
				inline ::std::valarray<double> const & get_Size() const {
					return _vertex;
				};
				inline ::std::valarray<double> const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(::std::valarray<double> const &point, ::std::valarray<double> const &vertex) {
					_glyph->Redraw(point, vertex);
				};
				inline void Rescale(::std::valarray<double> const &vertex) {
					_vertex = _scale(_baseVertex, vertex) * _baseVertex;
					_glyph->Rescale(_vertex);
				};
				inline void Move(::std::valarray<double> const &point) {
					_point = point;
					_glyph->Move(point);
				};
				inline void Focus(Focus mode) {

				};
				inline bool MouseLDown(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseLDouble(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseLUp(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseMDown(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseMDouble(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseRDown(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseRDouble(::std::valarray<double> const &point) {
					return true;
				};
				inline bool MouseWheel(::std::valarray<double> const &point, int delta) {
					return true;
				};
				inline bool MouseMove(::std::valarray<double> const &point) {
					return true;
				};

			private:
				::std::valarray<double> _point;
				::std::valarray<double> _vertex;
				::std::valarray<double> _baseVertex;
				TState _state;
				::std::unique_ptr<TGlyph> _glyph;
				TChangeNotify _changeNotify;
				TStateDefault _stateChange;
				TGlyphUpdate _glyphUpdate;
				TScale _scale;

				inline void OnSet(bool notify) {
					if (notify) {
						_changeNotify(_state);
					}
					_glyphUpdate(*_glyph, _state);
				};
			};
		}
	}
}

#endif
