#ifndef VOICEMEETER_UI_CONTROLS_STATE_HPP
#define VOICEMEETER_UI_CONTROLS_STATE_HPP

#include <memory>
#include <tuple>
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
					::std::valarray<int> &&baseVertex,
					::std::unique_ptr<TGlyph> &&glyph,
					TChangeNotify &&changeNotify,
					TStateChange &&stateChange = TStateChange{},
					TGlyphUpdate &&glyphUpdate = TGlyphUpdate{},
					TScale &&scale = TScale{})
					: _point{ 0, 0 }
					, _vertex{ 0, 0 }
					, _baseVertex{ ::std::move(baseVertex) }
					, _state{}
					, _glyph{ ::std::move(pGlyph) }
					, _changeNotify{ changeNotify }
					, _stateChange{ ::std::move(stateChange) }
					, _glyphUpdate{ ::std::move(glyphUpdate) }
					, _scale{ ::std::move(scale) } {

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

				inline void set_DefaultState() {
					_stateChange.SetDefault(_state);
					OnSet();
				};
				inline void set_NextState() {
					_stateChange.SetNext(_state);
					OnSet();
				};
				inline void set_PreviousState() {
					_stateChange.SetPrevious(_state);
					OnSet();
				};
				inline void set_State(TState &&value) {
					_stateChange.Set(_state, value);
					OnSet();
				};

				inline ::std::valarray<int> const & get_Position() const {
					return _point;
				};
				inline ::std::valarray<int> const & get_Size() const {
					return _vertex;
				};
				inline ::std::valarray<int> const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) {
					_glyph->Redraw(point, vertex);
				};
				inline void Rescale(::std::valarray<int> const &vertex) {
					::std::tie(_vertex) = _scale(vertex, _baseVertex);
					_glyph->Rescale(_vertex);
				};
				inline void Move(::std::valarray<int> const &point) {
					_point = point;
					_glyph->Move(point);
				};
				inline void Focus(Focus mode) {

				};
				inline bool MouseLDown(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseLDouble(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseLUp(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseMDown(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseMDouble(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseRDown(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseRDouble(::std::valarray<int> const &point) {
					return false;
				};
				inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
					return false;
				};
				inline bool MouseMove(::std::valarray<int> const &point) {
					return false;
				};

			private:
				::std::valarray<int> _point;
				::std::valarray<int> _vertex;
				::std::valarray<int> _baseVertex;
				TState _state;
				::std::unique_ptr<TGlyph> _glyph;
				TChangeNotify _changeNotify;
				TStateDefault _stateChange;
				TGlyphUpdate _glyphUpdate;
				TScale _scale;

				inline void OnSet() {
					_changeNotify(_state);
					_glyphUpdate(*_glyph, _state);
				};
			};
		}
	}
}

#endif
