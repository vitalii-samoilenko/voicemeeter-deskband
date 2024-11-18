#pragma once

#include <type_traits>
#include <utility>

#include "Environment/ITimer.h"

#include "../../Direction.h"
#include "../../IFocusTracker.h"
#include "../../IInputTracker.h"
#include "../../Controls/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<UI::Direction Direction, typename TKnob, typename TGlyph>
				class Knob : public TKnob {
					static_assert(
						::std::is_base_of_v<Controls::Knob<TGlyph>, TKnob>,
						"TKnob must be derived from Knob<TGlyph>");

				public:
					template<typename... Args>
					explicit Knob(
						IInputTracker& inputTracker,
						IFocusTracker& focusTracker,
						::Environment::ITimer& timer,
						Args&& ...args
					) : TKnob{ ::std::forward<Args>(args)... }
					  , m_inputTracker{ inputTracker }
					  , m_focusTracker{ focusTracker }
					  , m_timer{ timer } {

					}
					Knob() = delete;
					Knob(const Knob&) = delete;
					Knob(Knob&&) = delete;

					~Knob() = default;

					Knob& operator=(const Knob&) = delete;
					Knob& operator=(Knob&&) = delete;

					virtual void set_Focus(bool value) override {
						if (!value) {
							States::Knob state{ TKnob::get_State() };
							state.hold = false;
							TKnob::Set(state, false);
						}
					};

					virtual bool MouseLDown(const ::std::valarray<double>& point) override {
						m_inputTracker.set_Track(*this, true);
						m_inputTracker.set_Position(point);
						m_focusTracker.set_Track(*this, true);

						States::Knob state{ TKnob::get_State() };
						state.hold = true;
						TKnob::Set(state, false);

						return true;
					};
					virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
						TKnob::SetDefault();

						ReleaseLater();

						return true;
					};
					virtual bool MouseMDown(const ::std::valarray<double>& point) override {
						m_focusTracker.set_Track(*this, true);

						States::Knob state{ TKnob::get_State() };
						state.toggle = !state.toggle;
						TKnob::Set(state, true);

						return true;
					};
					virtual bool MouseMDouble(const ::std::valarray<double>& point) override {
						States::Knob state{ TKnob::get_State() };
						state.toggle = !state.toggle;
						TKnob::Set(state, true);

						return true;
					};
					virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
						m_focusTracker.set_Track(*this, true);

						States::Knob state{ TKnob::get_State() };
						state.gain += delta * 7;
						state.hold = true;
						TKnob::Set(state, true);

						ReleaseLater();

						return true;
					};
					virtual bool MouseMove(const ::std::valarray<double>& point) override {
						if (!m_inputTracker.get_Track(*this)) {
							return true;
						}

						double scale{ TKnob::get_Size()[Direction] / TKnob::get_BaseSize()[Direction] };
						States::Knob state{ TKnob::get_State() };
						state.gain += static_cast<int>((point[Direction] - m_inputTracker.get_Position()[Direction]) * 100. / scale);
						TKnob::Set(state, true);

						m_inputTracker.set_Position(point);

						return true;
					};
					virtual bool MouseLUp(const ::std::valarray<double>& point) override {
						m_inputTracker.set_Track(*this, false);

						ReleaseLater();

						return true;
					};

				private:
					IInputTracker& m_inputTracker;
					IFocusTracker& m_focusTracker;
					::Environment::ITimer& m_timer;

					inline void ReleaseLater() {
						m_timer.Set(::std::chrono::milliseconds{ 400 },
							[this]()->bool {
								if (m_inputTracker.get_Track(*this)) {
									return false;
								}

								States::Knob state{ TKnob::get_State() };
								state.hold = false;
								TKnob::Set(state, true);

								return false;
							});
					};
				};
			}
		}
	}
}