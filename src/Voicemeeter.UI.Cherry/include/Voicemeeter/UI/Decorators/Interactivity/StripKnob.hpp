#ifndef VOICEMEETER_UI_DECORATORS_INTERACTIVITY_STRIPKNOB_HPP
#define VOICEMEETER_UI_DECORATORS_INTERACTIVITY_STRIPKNOB_HPP

#include <chrono>
#include <utility>
#include <valarray>

#include "Voicemeeter/UI/Focus.hpp"
#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<
					typename TStripKnob,
					typename TTimer,
					typename TDirection>
				class StripKnob : public TStripKnob {
				public:
					template<typename... Args>
					inline StripKnob(
						TTimer &timer,
						TDirection &&direction = TDirection{},
						Args &&...args)
						: TStripKnob{ ::std::forward<Args>(args)... }
						, _timer{ timer }
						, _direction{ ::std::move(direction) }
						, _initPoint{ 0, 0 } {

					};
					StripKnob() = delete;
					StripKnob(StripKnob const &) = delete;
					StripKnob(StripKnob &&) = delete;

					inline ~StripKnob() = default;

					StripKnob & operator=(StripKnob const &) = delete;
					StripKnob & operator=(StripKnob &&) = delete;

					inline void Focus(Focus mode) {
						if (mode != Focus::None) {
							return;
						}
						set_HoldState(false);
					};
					inline bool MouseLDown(::std::valarray<int> const &point) {
						set_HoldState(true);
						_initPoint = point;
						return true;
					};
					inline bool MouseLDouble(::std::valarray<int> const &point) {
						TStripKnob::set_DefaultState();
						SetRelease();
						return true;
					};
					inline bool MouseLUp(::std::valarray<int> const &point) {
						SetRelease();
						return true;
					};
					inline bool MouseMDown(::std::valarray<int> const &point) {
						toggle_MuteState();
						return true;
					};
					inline bool MouseMDouble(::std::valarray<int> const &point) {
						toggle_MuteState();
						return true;
					};
					inline bool MouseRDown(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseRDouble(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
						add_GainState(delta);
						return true;
					};
					inline bool MouseMove(::std::valarray<int> const &point) {
						add_GainState(
							_direction(point - _initPoint)
							.sum());
						_initPoint = point;
						return true;
					};

				private:
					TTimer &_timer;
					TDirection _direction;
					::std::valarray<int> _initPoint;

					inline void set_HoldState(bool value) {
						State::StripKnob state{ TStripKnob::get_State() };
						state.hold = value;
						TStripKnob::set_State(state);
					};
					inline void toggle_MuteState() {
						State::StripKnob state{ TStripKnob::get_State() };
						state.toggle = !state.toggle;
						TStripKnob::set_State(state);
					};
					inline void add_GainState(int value) {
						State::StripKnob state{ TStripKnob::get_State() };
						state.degree += value;
						TStripKnob::set_State(state);
					};

					inline void SetRelease() {
						_timer.Set(::std::chrono::milliseconds{ 2000 },
							[this]()->void {
								set_HoldState(false);
								return false;
							});
					};
				};
			}
		}
	}
}

#endif
