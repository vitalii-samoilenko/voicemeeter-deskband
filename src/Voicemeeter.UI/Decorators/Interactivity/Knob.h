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
							state.pinned = false;
							TKnob::Set(state, false);
						}
					};

					virtual bool MouseLDown(const ::linear_algebra::vectord& point) override {
						m_inputTracker.set_Track(*this, true);
						m_inputTracker.set_Position(point);
						m_focusTracker.set_Track(*this, true);

						States::Knob state{ TKnob::get_State() };
						state.pinned = true;
						TKnob::Set(state, false);

						return true;
					};
					virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override {
						TKnob::SetDefault();

						UnpinLater();

						return true;
					};
					virtual bool MouseMDown(const ::linear_algebra::vectord& point) override {
						m_focusTracker.set_Track(*this, true);

						States::Knob state{ TKnob::get_State() };
						state.enabled = !state.enabled;
						TKnob::Set(state, true);

						return true;
					};
					virtual bool MouseMDouble(const ::linear_algebra::vectord& point) override {
						States::Knob state{ TKnob::get_State() };
						state.enabled = !state.enabled;
						TKnob::Set(state, true);

						return true;
					};
					virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override {
						m_focusTracker.set_Track(*this, true);

						States::Knob state{ TKnob::get_State() };
						state.gain += delta * 7;
						state.pinned = true;
						TKnob::Set(state, true);

						UnpinLater();

						return true;
					};
					virtual bool MouseMove(const ::linear_algebra::vectord& point) override {
						if (!m_inputTracker.get_Track(*this)) {
							return true;
						}

						OnMouseMove(point, UI::Direction_Tag<Direction>{});

						m_inputTracker.set_Position(point);

						return true;
					};
					virtual bool MouseLUp(const ::linear_algebra::vectord& point) override {
						m_inputTracker.set_Track(*this, false);

						UnpinLater();

						return true;
					};

				private:
					IInputTracker& m_inputTracker;
					IFocusTracker& m_focusTracker;
					::Environment::ITimer& m_timer;

					void OnMouseMove(const ::linear_algebra::vectord& point, const UI::Direction_Tag<UI::Direction::Right>&) {
						double scale{ TKnob::get_Size().x / TKnob::get_BaseSize().x };
						States::Knob state{ TKnob::get_State() };
						state.gain += static_cast<int>((point.x - m_inputTracker.get_Position().x) * 100. / scale);
						TKnob::Set(state, true);
					}

					void OnMouseMove(const ::linear_algebra::vectord& point, const Direction_Tag<UI::Direction::Down>&) {
						double scale{ TKnob::get_Size().y / TKnob::get_BaseSize().y };
						States::Knob state{ TKnob::get_State() };
						state.gain += static_cast<int>((point.y - m_inputTracker.get_Position().y) * 100. / scale);
						TKnob::Set(state, true);
					}

					void UnpinLater() {
						m_timer.Set(::std::chrono::milliseconds{ 400 },
							[this]()->bool {
								if (m_inputTracker.get_Track(*this)) {
									return false;
								}

								States::Knob state{ TKnob::get_State() };
								state.pinned = false;
								TKnob::Set(state, true);

								return false;
							});
					};
				};
			}
		}
	}
}