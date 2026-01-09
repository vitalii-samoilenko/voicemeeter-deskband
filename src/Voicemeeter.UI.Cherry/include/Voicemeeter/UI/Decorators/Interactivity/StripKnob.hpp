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
						, _releaseTick{ timer, *this }
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
						_releaseTick();
					};
					inline bool MouseLDown(::std::valarray<int> const &point) {
						_releaseTick.Unset();
						TStripKnob::set_HoldState(true);
						_initPoint = point;
						return true;
					};
					inline bool MouseLDouble(::std::valarray<int> const &point) {
						_releaseTick.Set();
						TStripKnob::set_DefaultState();
						return true;
					};
					inline bool MouseLUp(::std::valarray<int> const &point) {
						_releaseTick.Set();
						return true;
					};
					inline bool MouseMDown(::std::valarray<int> const &point) {
						TStripKnob::toggle_MuteState();
						return true;
					};
					inline bool MouseMDouble(::std::valarray<int> const &point) {
						TStripKnob::toggle_MuteState();
						return true;
					};
					inline bool MouseRDown(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseRDouble(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
						_releaseTick.Set();
						States::StripKnob state{ TStripKnob::get_State() };
						state.hold = true;
						state.degree += delta;
						TStripKnob::set_State(state);
						return true;
					};
					inline bool MouseMove(::std::valarray<int> const &point) {
						TStripKnob::add_GainState(
							_direction(point - _initPoint)
							.sum());
						_initPoint = point;
						return true;
					};

				private:
					class ReleaseTick final {
					public:
						inline ReleaseTick(
							TTimer &timer,
							TStripKnob &stripKnob)
							: _timer{ timer }
							, _stripKnob{ stripKnob } {

						};
						ReleaseTick() = delete;
						ReleaseTick(ReleaseTick const &) = delete;
						ReleaseTick(ReleaseTick &&) = delete;

						inline ~ReleaseTick() = default;

						ReleaseTick & operator=(ReleaseTick const &) = delete;
						ReleaseTick & operator=(ReleaseTick &&) = delete;

						inline void operator()() const {
							Unset();
							_stripKnob.set_HoldState(false);
						};

						inline void Set() {
							_timer.Set(
								::std::chrono::milliseconds{ 2000 },
								*this);
						};
						inline void Unset() {
							_timer.Unset(
								*this);
						};

					private:
						TTimer &_timer;
						TStripKnob &_stripKnob;
					};

					ReleaseTick _releaseTick;
					TDirection _direction;
					::std::valarray<int> _initPoint;
				};
			}
		}
	}
}

#endif
