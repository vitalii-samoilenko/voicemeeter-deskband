#ifndef VOICEMEETER_UI_DECORATORS_HITTEST_HPP
#define VOICEMEETER_UI_DECORATORS_HITTEST_HPP

#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			template<typename TComponent>
			class HitTest : public TComponent {
			public:
				template<typename... Args>
				inline explicit HitTest(Args &&...args)
					: TComponent{ ::std::forward<Args>(args)... } {

				};
				HitTest(HitTest const &) = delete;
				HitTest(HitTest &&) = delete;

				inline ~HitTest() = default;

				HitTest & operator=(HitTest const &) = delete;
				HitTest & operator=(HitTest &&) = delete;

				inline void Redraw(vector_t const &point, vector_t const &vertex) {
					if (!IsOverlapping(point, vertex)) {
						return;
					}
					TComponent::Redraw(point, vertex);
				};
				inline bool MouseLDown(vector_t const &point) {
					return IsInside(point) && TComponent::MouseLDown(point);
				};
				inline bool MouseLDouble(vector_t const &point) {
					return IsInside(point) && TComponent::MouseLDouble(point);
				};
				inline bool MouseLUp(vector_t const &point) {
					return IsInside(point) && TComponent::MouseLUp(point);
				};
				inline bool MouseMDown(vector_t const &point) {
					return IsInside(point) && TComponent::MouseMDown(point);
				};
				inline bool MouseMDouble(vector_t const &point) {
					return IsInside(point) && TComponent::MouseMDouble(point);
				};
				inline bool MouseRDown(vector_t const &point) {
					return IsInside(point) && TComponent::MouseRDown(point);
				};
				inline bool MouseRDouble(vector_t const &point) {
					return IsInside(point) && TComponent::MouseRDouble(point);
				};
				inline bool MouseWheel(vector_t const &point, num_t delta) {
					return IsInside(point) && TComponent::MouseWheel(point, delta);
				};
				inline bool MouseMove(vector_t const &point) {
					return IsInside(point) && TComponent::MouseMove(point);
				};

			private:
				static inline bool is_inside(
					vector_t const &point, vector_t const &vertex) {
					return max(point - vertex) < 0;
				};
				static inline bool is_overlapping(
					vector_t const &lhs_point, vector_t const &lhs_vertex,
					vector_t const &rhs_point, vector_t const &rhs_vertex) {
					return is_inside(rhs_point, lhs_point + lhs_vertex)
						&& is_inside(lhs_point, rhs_point + rhs_vertex);
				};
				inline bool IsOverlapping(
					vector_t const &point, vector_t const &vertex) const {
					return is_overlapping(
							point, vertex,
							TComponent::get_Position(), TComponent::get_Size());
				};
				inline bool IsInside(vector_t const &point) const {
					return is_inside(
						point - TComponent::get_Position(), TComponent::get_Size());
				};
			};
		}
	}
}

#endif
