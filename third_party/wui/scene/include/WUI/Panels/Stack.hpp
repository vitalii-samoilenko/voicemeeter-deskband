#ifndef WUI_PANELS_STACK_HPP
#define WUI_PANELS_STACK_HPP

#include <memory>
#include <tuple>
#include <utility>

#include "wheel.hpp"

#include "WUI/Focus.hpp"

namespace WUI {
	namespace Panels {
		template<
			typename TDirection,
			typename TScale,
			typename ...TComponents>
		class Stack {
		public:
			inline Stack(
				vec_t &&baseVertex,
				TDirection &&direction = TDirection{},
				TScale &&scale = TScale{},
				::std::unique_ptr<TComponents> &&...components)
				: _direction{ ::std::move(direction) }
				, _scale{ ::std::move(scale) }
				, _components{ ::std::move(components) ... }
				, _vertex{ 0, 0 }
				, _baseVertex{ ::std::move(baseVertex) } {

			};
			Stack() = delete;
			Stack(Stack const &) = delete;
			Stack(Stack &&) = delete;

			inline ~Stack() = default;

			Stack & operator=(Stack const &) = delete;
			Stack & operator=(Stack &&) = delete;

			inline vec_t const & get_Position() const {
				return ::std::get<0>(_components)
					->get_Position();
			};
			inline vec_t const & get_Size() const {
				return _vertex;
			};
			inline vec_t const & get_BaseSize() const {
				return _baseVertex;
			};

			inline void set_Focus(Focus value) {
				::std::get<0>(_components)
					->set_Focus(value);
			};

			inline void Redraw(vec_t const &point, vec_t const &vertex) {
				::std::apply([
					&point,
					&vertex
				](::std::unique_ptr<TComponents> &...components)->void {
					(components->Redraw(point, vertex)
					, ...);
				}, _components);
			};
			inline void Rescale(vec_t const &vertex) {
				::std::tie(_vertex) = _scale(vertex, _baseVertex);
				::std::apply([
					&direction = _direction,
					vertex = _vertex
				](::std::unique_ptr<TComponents> &...components) mutable ->void {
					((components->Rescale(vertex)
					,(vertex -= direction(components->get_Size())))
					, ...);
				}, _components);
				Move(::std::get<0>(_components)
					->get_Position());
			};
			inline void Move(vec_t const &point) {
				::std::apply([
					&direction = _direction,
					point = point
				](::std::unique_ptr<TComponents> &...components) mutable ->void {
					((components->Move(point)
					,(point += direction(components->get_Size())))
					, ...);
				}, _components);
			};
			inline bool MouseLDown(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseLDown(point) || ...);
				}, _components);
			};
			inline bool MouseLDouble(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseLDouble(point) || ...);
				}, _components);
			};
			inline bool MouseLUp(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseLUp(point) || ...);
				}, _components);
			};
			inline bool MouseMDown(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseMDown(point) || ...);
				}, _components);
			};
			inline bool MouseMDouble(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseMDouble(point) || ...);
				}, _components);
			};
			inline bool MouseRDown(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseRDown(point) || ...);
				}, _components);
			};
			inline bool MouseRDouble(vec_t const &point) {
				return ::std::apply([
					&point
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseRDouble(point) || ...);
				}, _components);
			};
			inline bool MouseWheel(vec_t const &point, num_t delta) {
				return ::std::apply([
					&point,
					delta
				](::std::unique_ptr<TComponents> &...components)->bool {
					return (components->MouseWheel(point, delta) || ...);
				}, _components);
			};
			inline bool MouseMove(vec_t const &point) {
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
			vec_t _vertex;
			vec_t _baseVertex;
		};
	}
}

#endif
