#ifndef VOICEMEETER_DECORATORS_COMPONENT_HITTEST_HPP
#define VOICEMEETER_DECORATORS_COMPONENT_HITTEST_HPP

#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Component {
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

					inline void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) {
						if (!IsOverlapping(point, vertex)) {
							return;
						}
						TComponent::Redraw(point, vertex);
					};
					inline bool MouseLDown(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseLDown(point);
					};
					inline bool MouseLDouble(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseLDouble(point);
					};
					inline bool MouseLUp(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseLUp(point);
					};
					inline bool MouseMDown(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseMDown(point);
					};
					inline bool MouseMDouble(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseMDouble(point);
					};
					inline bool MouseRDown(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseRDown(point);
					};
					inline bool MouseRDouble(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseRDouble(point);
					};
					inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
						return IsInside(point) && TComponent::MouseWheel(point, delta);
					};
					inline bool MouseMove(::std::valarray<int> const &point) {
						return IsInside(point) && TComponent::MouseMove(point);
					};

				private:
					static inline bool is_inside(
						::std::valarray<int> const &point,
						::std::valarray<int> const &vertex) {
						return (point - vertex).max() < 0;
					};
					static inline bool is_overlapping(
						::std::valarray<int> const &lhs_point,
						::std::valarray<int> const &lhs_vertex,
						::std::valarray<int> const &rhs_point,
						::std::valarray<int> const &rhs_vertex) {
						return is_inside(rhs_point, lhs_point + lhs_vertex)
							&& is_inside(lhs_point, rhs_point + rhs_vertex);
					};
					inline bool IsOverlapping(
						::std::valarray<int> const &point,
						::std::valarray<int> const &vertex) const {
						return is_overlapping(
								point, vertex,
								TComponent::get_Position(), TComponent::get_Size());
					};
					inline bool IsInside(
						::std::valarray<int> const &point) const {
						return is_inside(
							point - TComponent::get_Position(),
							TComponent::get_Size());
					};
				};
			}
		}
	}
}

#endif
