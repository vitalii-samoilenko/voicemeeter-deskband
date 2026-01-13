#ifndef VOICEMEETER_UI_PANELS_STACK_HPP
#define VOICEMEETER_UI_PANELS_STACK_HPP

#include <memory>
#include <tuple>
#include <utility>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Panels {
			template<
				typename TDirection,
				typename TScale,
				typename... TComponents>
			class Stack {
			public:
				inline Stack(
					vector_t &&baseVertex,
					TDirection &&direction = TDirection{},
					TScale &&scale = TScale{},
					::std::unique_ptr<TComponents> &&...components)
					: _direction{ ::std::move(direction) }
					, _scale{ ::std::move(scale) }
					, _components{ ::std::move(components)... }
					, _vertex{ 0, 0 }
					, _baseVertex{ ::std::move(baseVertex) } {

				};
				Stack() = delete;
				Stack(Stack const &) = delete;
				Stack(Stack &&) = delete;

				inline ~Stack() = default;

				Stack & operator=(Stack const &) = delete;
				Stack & operator=(Stack &&) = delete;

				inline vector_t const & get_Position() const {
					return ::std::get<0>(_components)
						->get_Position();
				};
				inline vector_t const & get_Size() const {
					return _vertex;
				};
				inline vector_t const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Redraw(vector_t const &point, vector_t const &vertex) {
					::std::apply([
						&point,
						&vertex
					](::std::unique_ptr<TComponents> &...components)->void {
						(components->Redraw(point, vertex)
						, ...);
					}, _components);
				};
				inline void Rescale(vector_t const &vertex) {
					::std::tie(_vertex) = _scale(vertex, _baseVertex);
					::std::apply([
						&direction = _direction,
						vertex = _vertex
					](::std::unique_ptr<TComponents> &...components)->void {
						((components->Rescale(vertex)
						,(vertex -= direction(components->get_Size())))
						, ...);
					}, _components);
					Move(::std::get<0>(_components)
						->get_Position());
				};
				inline void Move(vector_t const &point) {
					::std::apply([
						&direction = _direction,
						point
					](::std::unique_ptr<TComponents> &...components)->void {
						((components->Move(point)
						,(point += direction(components->get_Size())))
						, ...);
					}, _components);
				};
				inline void Focus(Focus mode) {
					::std::get<0>(_components)
						->Focus(mode);
				};
				inline bool MouseLDown(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLDown(point) || ...);
					}, _components);
				};
				inline bool MouseLDouble(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLDouble(point) || ...);
					}, _components);
				};
				inline bool MouseLUp(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseLUp(point) || ...);
					}, _components);
				};
				inline bool MouseMDown(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMDown(point) || ...);
					}, _components);
				};
				inline bool MouseMDouble(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMDouble(point) || ...);
					}, _components);
				};
				inline bool MouseRDown(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseRDown(point) || ...);
					}, _components);
				};
				inline bool MouseRDouble(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseRDouble(point) || ...);
					}, _components);
				};
				inline bool MouseWheel(vector_t const &point, num_t delta) {
					return ::std::apply([
						&point,
						delta
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseWheel(point, delta) || ...);
					}, _components);
				};
				inline bool MouseMove(vector_t const &point) {
					return ::std::apply([
						&point
					](::std::unique_ptr<TComponents> &...components)->bool {
						return (components->MouseMove(point) || ...);
					}, _components);
				};

			private:
				TDirection _direction;
				TScale _scale;
				::std::tuple<::std::unique_ptr<TComponents> ...> _components;
				vector_t _vertex;
				vector_t _baseVertex;
			};
		}
	}
}

#endif
