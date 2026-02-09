#ifndef VOICEMEETER_UI_DECORATORS_INTERACTIVITY_POINT_HPP
#define VOICEMEETER_UI_DECORATORS_INTERACTIVITY_POINT_HPP

#include <utility>

#include "wheel.hpp"

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<
					typename TComponent,
					typename TFocusTracker>
				class Point : public TComponent {
				public:
					template<typename ...Args>
					inline explicit Point(
						TFocusTracker &focusTracker,
						Args &&...args)
						: TComponent{ ::std::forward<Args>(args) ... }
						, _focusTracker{ focusTracker } {

					};
					Point() = delete;
					Point(Point const &) = delete;
					Point(Point &&) = delete;

					inline ~Point() = default;

					Point & operator=(Point const &) = delete;
					Point & operator=(Point &&) = delete;

					inline bool MouseLDown(vector_t const &point) {
						bool base{ TComponent::MouseLDown(point) };
						if (base) {
							_focusTracker.set_Track(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseLDouble(vector_t const &point) {
						bool base{ TComponent::MouseLDouble(point) };
						if (base) {
							_focusTracker.set_Track(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseLUp(vector_t const &point) {
						return TComponent::MouseLUp(point);
					};
					inline bool MouseMDown(vector_t const &point) {
						bool base{ TComponent::MouseMDown(point) };
						if (base) {
							_focusTracker.set_Track(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseMDouble(vector_t const &point) {
						bool base{ TComponent::MouseMDouble(point) };
						if (base) {
							_focusTracker.set_Track(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseRDown(vector_t const &point) {
						bool base{ TComponent::MouseRDown(point) };
						if (base) {
							_focusTracker.set_Track(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseRDouble(vector_t const &point) {
						bool base{ TComponent::MouseRDouble(point) };
						if (base) {
							_focusTracker.set_Track(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseWheel(vector_t const &point, num_t delta) {
						return TComponent::MouseWheel(point, delta);
					};
					inline bool MouseMove(vector_t const &point) {
						return TComponent::MouseMove(point);
					};

				private:
					TFocusTracker &_focusTracker;
				};
			}
		}
	}
}

#endif
