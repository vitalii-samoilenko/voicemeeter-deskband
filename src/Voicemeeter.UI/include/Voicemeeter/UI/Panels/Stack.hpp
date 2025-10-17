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
					, _componentsBaseVertex{
						::std::apply([
							&direction = _direction,
							dimension = ::std::get<0>(_components)
								->get_BaseSize()
									.size()
						](::std::unique_ptr<TComponents> &...components)->::std::valarray<double> {
							normalization(direction, dimension, components...);
							::std::valarray<double> componentsBaseVertex(0, dimension * sizeof...(TComponents));
							size_t i{ 0 };
							(((componentsBaseVertex[
								::std::slice(i, dimension, sizeof...(TComponents))
							] = components->get_Size()), (++i)), ...);
							return componentsBaseVertex;
						}, _components)
					}
					, _vertex{
						::std::apply([
							&direction = _direction,
							dimension = ::std::get<0>(_components)
								->get_BaseSize()
									.size()
						](::std::unique_ptr<TComponents> &...componets)->::std::valarray<double> {
							::std::valarray<double> vertex(0, dimension);
							(((vertex += direction(components->get_Size())),
							(vertex[vertex < components->get_Size()] = components->get_Size())), ...);
							return vertex;
						}, _components);
					}
					, _baseVertex{ _vertex } {

				};
				Stack() = delete;
				Stack(Stack const &) = delete;
				Stack(Stack &&) = delete;

				inline ~Stack() = default;

				Stack & operator=(Stack const &) = delete;
				Stack & operator=(Stack &&) = delete;

				inline ::std::valarray<double> const & get_Position() const {
					return ::std::get<0>(_components)
						->get_Position();
				};
				inline ::std::valarray<double> const & get_Size() const {
					return _vertex;
				};
				inline ::std::valarray<double> const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(::std::valarray<double> const &point, ::std::valarray<double> const &vertex) {
					::std::apply([&point, &vertex](::std::unique_ptr<TComponents> &...components)->void {
						(components->Redraw(point, vertex), ...);
					}, _components);
				};
				inline void Rescale(::std::valarray<double> const &vertex) {
					::std::valarray<double> scale{ _scale(_baseVertex, vertex) };
					::std::apply([
						&scale,
						&componentsBaseVertex = _componentsBaseVertex,
						dimension = _baseVertex.size()
					](::std::unique_ptr<TComponents> &...components)->void {
						size_t i{ 0 };
						((components->Rescale(scale * componentsBaseVertex[
							::std::slice(i, dimension, sizeof...(TComponents))
						]), (++i)), ...);
					}, _components);
					Move(::std::get<0>(_components)
						->get_Position());
					_vertex = scale * _baseVertex;
				};
				inline void Move(::std::valarray<double> const &point) {
					::std::apply([&direction = _direction, point](::std::unique_ptr<TComponents> &...components)->void {
						((components->Move(point), (point += direction(components->get_Size()))), ...);
					}, _components);
				};
				inline void Focus(Focus mode) {
					::std::get<0>(_components)
						->Focus(mode);
				};
				inline bool MouseLDown(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLDown(point) || ...);
					}, _components);
				};
				inline bool MouseLDouble(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLDouble(point) || ...);
					}, _components);
				};
				inline bool MouseLUp(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLUp(point) || ...);
					}, _components);
				};
				inline bool MouseMDown(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMDown(point) || ...);
					}, _components);
				};
				inline bool MouseMDouble(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMDouble(point) || ...);
					}, _components);
				};
				inline bool MouseRDown(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseRDown(point) || ...);
					}, _components);
				};
				inline bool MouseRDouble(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseRDouble(point) || ...);
					}, _components);
				};
				inline bool MouseWheel(::std::valarray<double> const &point, int delta) {
					return ::std::apply([&point, delta](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseWheel(point, delta) || ...);
					}, _components);
				};
				inline bool MouseMove(::std::valarray<double> const &point) {
					return ::std::apply([&point](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMove(point) || ...);
					}, _components);
				};

			private:
				TDirection _direction;
				TScale _scale;
				::std::tuple<::std::unique_ptr<TComponents>...> _components;
				::std::valarray<double> _componentsBaseVertex;
				::std::valarray<double> _vertex;
				::std::valarray<double> _baseVertex;
			};
		}
	}
}

#endif
