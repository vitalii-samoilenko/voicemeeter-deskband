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
				template<typename ...Args>
				inline explicit HitTest(Args &&...args)
					: TComponent{ ::std::forward<Args>(args) ... } {

				};
				HitTest(HitTest const &) = delete;
				HitTest(HitTest &&) = delete;

				inline ~HitTest() = default;

				HitTest & operator=(HitTest const &) = delete;
				HitTest & operator=(HitTest &&) = delete;

				inline void Redraw(vec_t const &point, vec_t const &vertex) {
					if (!IsOverlapping(point, vertex)) {
						return;
					}
					TComponent::Redraw(point, vertex);
				};
				inline bool MouseLDown(vec_t const &point) {
					return IsInside(point) && TComponent::MouseLDown(point);
				};
				inline bool MouseLDouble(vec_t const &point) {
					return IsInside(point) && TComponent::MouseLDouble(point);
				};
				inline bool MouseLUp(vec_t const &point) {
					return IsInside(point) && TComponent::MouseLUp(point);
				};
				inline bool MouseMDown(vec_t const &point) {
					return IsInside(point) && TComponent::MouseMDown(point);
				};
				inline bool MouseMDouble(vec_t const &point) {
					return IsInside(point) && TComponent::MouseMDouble(point);
				};
				inline bool MouseRDown(vec_t const &point) {
					return IsInside(point) && TComponent::MouseRDown(point);
				};
				inline bool MouseRDouble(vec_t const &point) {
					return IsInside(point) && TComponent::MouseRDouble(point);
				};
				inline bool MouseWheel(vec_t const &point, num_t delta) {
					return IsInside(point) && TComponent::MouseWheel(point, delta);
				};
				inline bool MouseMove(vec_t const &point) {
					return IsInside(point) && TComponent::MouseMove(point);
				};

			private:
				inline bool IsOverlapping(
					vec_t const &point, vec_t const &vertex) const {
					return is_overlapping(
							point, vertex,
							TComponent::get_Position(), TComponent::get_Size());
				};
				inline bool IsInside(vec_t const &point) const {
					return is_inside(
						point - TComponent::get_Position(), TComponent::get_Size());
				};
			};
		}
	}
}

#endif
