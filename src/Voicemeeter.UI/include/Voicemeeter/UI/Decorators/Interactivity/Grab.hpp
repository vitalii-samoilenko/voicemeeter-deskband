#ifndef VOICEMEETER_UI_DECORATORS_INTERACTIVITY_GRAB_HPP
#define VOICEMEETER_UI_DECORATORS_INTERACTIVITY_GRAB_HPP

#include <utility>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<
					typename TComponent,
					typename TFocusTracker>
				class Grab : public TComponent {
				public:
					template<typename... Args>
					inline explicit Grab(
						TFocusTracker &focusTracker,
						Args &&...args)
						: TComponent{ ::std::forward<Args>(args)... }
						, _focusTracker{ focusTracker } {

					};
					Grab() = delete;
					Grab(Grab const &) = delete;
					Grab(Grab &&) = delete;

					inline ~Grab() = default;

					Grab & operator=(Grab const &) = delete;
					Grab & operator=(Grab &&) = delete;

					inline void Focus(Focus mode) {

					};
					inline bool MouseLDown(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseLDown(point) };
						if (base) {
							_focusTracker.set_Tracked(*this, Focus::Fixed);
						}
						return base;
					};
					inline bool MouseLDouble(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseLDouble(point) };
						if (base) {
							_focusTracker.set_Tracked(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseLUp(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseLUp(point) };
						if (base) {
							_focusTracked.set_Tracked(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseMDown(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseMDown(point) };
						if (base) {
							_focusTracker.set_Tracked(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseMDouble(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseMDouble(point) };
						if (base) {
							_focusTracker.set_Tracked(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseRDown(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseRDown(point) };
						if (base) {
							_focusTracker.set_Tracked(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseRDouble(::std::valarray<int> const &point) {
						bool base{ TComponent::MouseRDouble(point) };
						if (base) {
							_focusTracker.set_Tracked(*this, Focus::Floating);
						}
						return base;
					};
					inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
						return TComponent::MouseWheel(point, delta);
					};
					inline bool MouseMove(::std::valarray<int> const &point) {
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
