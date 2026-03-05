#ifndef WUI_DECORATORS_INTERACTIVITY_GRAB_HPP
#define WUI_DECORATORS_INTERACTIVITY_GRAB_HPP

#include <utility>

#include "math.hpp"

#include "WUI/Focus.hpp"

namespace UI {
	namespace Decorators {
		namespace Interactivity {
			template<
				typename TComponent,
				typename TFocusTracker>
			class Grab : public TComponent {
			public:
				template<typename ...Args>
				inline explicit Grab(
					TFocusTracker &focusTracker,
					Args &&...args)
					: TComponent{ ::std::forward<Args>(args) ... }
					, _focusTracker{ focusTracker } {

				};
				Grab() = delete;
				Grab(Grab const &) = delete;
				Grab(Grab &&) = delete;

				inline ~Grab() = default;

				Grab & operator=(Grab const &) = delete;
				Grab & operator=(Grab &&) = delete;

				inline bool MouseLDown(vec_t const &point) {
					bool base{ TComponent::MouseLDown(point) };
					if (base) {
						_focusTracker.set_Track(*this, Focus::Fixed);
					}
					return base;
				};
				inline bool MouseLDouble(vec_t const &point) {
					bool base{ TComponent::MouseLDouble(point) };
					if (base) {
						_focusTracker.set_Track(*this, Focus::Floating);
					}
					return base;
				};
				inline bool MouseLUp(vec_t const &point) {
					bool base{ TComponent::MouseLUp(point) };
					_focusTracker.down_Track(*this, Focus::Floating);
					return base;
				};
				inline bool MouseMDown(vec_t const &point) {
					bool base{ TComponent::MouseMDown(point) };
					if (base) {
						_focusTracker.up_Track(*this, Focus::Floating);
					}
					return base;
				};
				inline bool MouseMDouble(vec_t const &point) {
					bool base{ TComponent::MouseMDouble(point) };
					if (base) {
						_focusTracker.up_Track(*this, Focus::Floating);
					}
					return base;
				};
				inline bool MouseRDown(vec_t const &point) {
					bool base{ TComponent::MouseRDown(point) };
					if (base) {
						_focusTracker.up_Track(*this, Focus::Floating);
					}
					return base;
				};
				inline bool MouseRDouble(vec_t const &point) {
					bool base{ TComponent::MouseRDouble(point) };
					if (base) {
						_focusTracker.up_Track(*this, Focus::Floating);
					}
					return base;
				};
				inline bool MouseWheel(vec_t const &point, num_t delta) {
					return TComponent::MouseWheel(point, delta);
				};
				inline bool MouseMove(vec_t const &point) {
					return TComponent::MouseMove(point);
				};

			private:
				TFocusTracker &_focusTracker;
			};
		}
	}
}

#endif
