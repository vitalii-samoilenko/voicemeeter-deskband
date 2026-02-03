#ifndef VOICEMEETER_UI_DECORATORS_INTERACTIVITY_STRIPKNOB_HPP
#define VOICEMEETER_UI_DECORATORS_INTERACTIVITY_STRIPKNOB_HPP

#include <utility>

#include "wheel.hpp"

#include "Voicemeeter/UI/Focus.hpp"

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
					template<typename ...Args>
					inline StripKnob(
						TTimer &timer,
						TDirection &&direction = TDirection{},
						Args &&...args)
						: TStripKnob{ ::std::forward<Args>(args) ... }
						, _releaseTick{ this, timer }
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
						_releaseTick.Unset();
						TStripKnob::set_HoldState(false);
					};
					inline bool MouseLDown(vector_t const &point) {
						_releaseTick.Unset();
						TStripKnob::set_HoldState(true);
						_initPoint = point;
						return true;
					};
					inline bool MouseLDouble(vector_t const &point) {
						_releaseTick.Set();
						TStripKnob::set_DefaultState();
						return true;
					};
					inline bool MouseLUp(vector_t const &point) {
						_releaseTick.Set();
						return true;
					};
					inline bool MouseMDown(vector_t const &point) {
						TStripKnob::toggle_MuteState();
						return true;
					};
					inline bool MouseMDouble(vector_t const &point) {
						TStripKnob::toggle_MuteState();
						return true;
					};
					inline bool MouseRDown(vector_t const &point) {
						return true;
					};
					inline bool MouseRDouble(vector_t const &point) {
						return true;
					};
					inline bool MouseWheel(vector_t const &point, num_t delta) {
						_releaseTick.Set();
						TStripKnob::add_GainState(delta);
						return true;
					};
					inline bool MouseMove(vector_t const &point) {
						TStripKnob::add_GainState(
							sum(_direction(point - _initPoint)));
						_initPoint = point;
						return true;
					};

				private:
					class ReleaseTick final {
					public:
						inline ReleaseTick(
							StripKnob *that,
							TTimer &timer)
							: that{ that }
							, _timer{ timer } {

						};
						ReleaseTick() = delete;
						ReleaseTick(ReleaseTick const &) = delete;
						ReleaseTick(ReleaseTick &&) = delete;

						inline ~ReleaseTick() {
							Unset();
						};

						ReleaseTick & operator=(ReleaseTick const &) = delete;
						ReleaseTick & operator=(ReleaseTick &&) = delete;

						inline void operator()() const {
							Unset();
							that->set_HoldState(false);
						};

						inline void Set() {
							_timer.Set(2000, *this);
						};
						inline void Unset() {
							_timer.Unset(*this);
						};

					private:
						StripKnob *that;
						TTimer &_timer;
					};

					ReleaseTick _releaseTick;
					TDirection _direction;
					vector_t _initPoint;
				};
			}
		}
	}
}

#endif
