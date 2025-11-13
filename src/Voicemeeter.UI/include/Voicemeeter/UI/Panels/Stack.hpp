#ifndef VOICEMEETER_UI_PANELS_STACK_HPP
#define VOICEMEETER_UI_PANELS_STACK_HPP

#include <memory>
#include <tuple>
#include <utility>
#include <valarray>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Panels {
			template<
				typename TNormalization,
				typename TDirection,
				typename TScale,
				typename... TComponents>
			class Stack {
			public:
				inline Stack(
					TNormalization &&normalization = TNormalization{},
					TDirection &&direction = TDirection{},
					TScale &&scale = TScale{},
					::std::unique_ptr<TComponents> &&...components)
					: _direction{ ::std::move(direction) }
					, _scale{ ::std::move(scale) }
					, _components{ ::std::move(components)... }
					, _vertex{ 0, 0 }
					, _baseVertex{ 
						::std::apply([
							&normalization,
							&direction = _direction
						](::std::unique_ptr<TComponents> &...componets)->::std::valarray<int> {
							normalization(direction, *components...);
							::std::valarray<int> vertex{ 0, 0 };
							(((vertex += direction(components->get_Size())),
							(vertex[vertex < components->get_Size()] = components->get_Size())),
							...);
							return vertex;
						}, _components)
					} {

				};
				Stack() = delete;
				Stack(Stack const &) = delete;
				Stack(Stack &&) = delete;

				inline ~Stack() = default;

				Stack & operator=(Stack const &) = delete;
				Stack & operator=(Stack &&) = delete;

				inline ::std::valarray<int> const & get_Position() const {
					return ::std::get<0>(_components)
						->get_Position();
				};
				inline ::std::valarray<int> const & get_Size() const {
					return _vertex;
				};
				inline ::std::valarray<int> const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) {
					::std::apply([
						&point,
						&vertex
					](::std::unique_ptr<TComponents> &...components)->void {
						(components->Redraw(point, vertex), ...);
					}, _components);
				};
				inline void Rescale(::std::valarray<int> const &vertex) {
					::std::tie(_vertex) = _scale(vertex, _baseVertex);
					::std::apply([
						&direction = _direction,
						vertex = _vertex
					](::std::unique_ptr<TComponents> &...components)->void {
						((components->Rescale(vertex),
						(vertex -= _direction(components->get_Size()))),
						...);
					}, _components);
					Move(::std::get<0>(_components)
						->get_Position());
				};
				inline void Move(::std::valarray<int> const &point) {
					::std::apply([
						&direction = _direction,
						point
					](::std::unique_ptr<TComponents> &...components)->void {
						((components->Move(point),
						(point += direction(components->get_Size()))),
						...);
					}, _components);
				};
				inline void Focus(Focus mode) {
					::std::get<0>(_components)
						->Focus(mode);
				};
				inline bool MouseLDown(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLDown(point) || ...);
					}, _components);
				};
				inline bool MouseLDouble(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLDouble(point) || ...);
					}, _components);
				};
				inline bool MouseLUp(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLUp(point) || ...);
					}, _components);
				};
				inline bool MouseMDown(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMDown(point) || ...);
					}, _components);
				};
				inline bool MouseMDouble(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMDouble(point) || ...);
					}, _components);
				};
				inline bool MouseRDown(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseRDown(point) || ...);
					}, _components);
				};
				inline bool MouseRDouble(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseRDouble(point) || ...);
					}, _components);
				};
				inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
					return ::std::apply([
						&point,
						delta
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseWheel(point, delta) || ...);
					}, _components);
				};
				inline bool MouseMove(::std::valarray<int> const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMove(point) || ...);
					}, _components);
				};

			private:
				TDirection _direction;
				TScale _scale;
				::std::tuple<::std::unique_ptr<TComponents>...> _components;
				::std::valarray<int> _vertex;
				::std::valarray<int> _baseVertex;
			};
		}
	}
}

#endif
