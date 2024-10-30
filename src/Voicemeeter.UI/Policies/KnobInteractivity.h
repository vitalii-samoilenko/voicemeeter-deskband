#pragma once

#include "estd/linear_algebra.h"

#include "Environment/ITimer.h"

#include "../IFocusTracker.h"
#include "../IInputTracker.h"
#include "../Controls/Knob.h"
#include "IInteractivity.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TGlyph>
			class KnobInteractivity : public IInteractivity<Controls::Knob<TGlyph>> {
			public:
				explicit KnobInteractivity(
					IInputTracker& inputTracker,
					IFocusTracker& focusTracker,
					::Environment::ITimer& timer
				) : m_inputTracker{ inputTracker }
				  , m_focusTracker{ focusTracker }
				  , m_timer{ timer } {

				};
				KnobInteractivity() = delete;
				KnobInteractivity(const KnobInteractivity&) = delete;
				KnobInteractivity(KnobInteractivity&&) = delete;

				~KnobInteractivity() = default;

				KnobInteractivity& operator=(const KnobInteractivity&) = delete;
				KnobInteractivity& operator=(KnobInteractivity&&) = delete;

				virtual void set_Focus(Controls::Knob<TGlyph>& component, bool value) const override {
					if (!value) {
						States::Knob state{ component.get_State() };
						state.pinned = false;
						component.Set(state, false);
					}
				};

				virtual void MouseLDown(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.set_Track(component, true);
					m_inputTracker.set_Position(point);
					m_focusTracker.set_Track(component, true);

					States::Knob state{ component.get_State() };
					state.pinned = true;
					component.Set(state, false);
				};
				virtual void MouseLDouble(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					component.SetDefault();

					UnpinLater(component);
				};
				virtual void MouseRDown(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_focusTracker.set_Track(component, true);

					States::Knob state{ component.get_State() };
					state.enabled = !state.enabled;
					component.Set(state, true);
				};
				virtual void MouseRDouble(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					States::Knob state{ component.get_State() };
					state.enabled = !state.enabled;
					component.Set(state, true);
				};
				virtual void MouseWheel(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point, int delta) const override {
					m_focusTracker.set_Track(component, true);

					States::Knob state{ component.get_State() };
					state.gain += delta * 7;
					state.pinned = true;
					component.Set(state, true);

					UnpinLater(component);
				};
				virtual void MouseMove(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					if (!m_inputTracker.get_Track(component)) {
						return;
					}

					double scale{ component.get_Size().x / component.get_BaseSize().x };
					States::Knob state{ component.get_State() };
					state.gain += static_cast<int>((point.x - m_inputTracker.get_Position().x) * 100. / scale);
					component.Set(state, true);

					m_inputTracker.set_Position(point);
				};
				virtual void MouseLUp(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.set_Track(component, false);

					UnpinLater(component);
				};

			private:
				IFocusTracker& m_focusTracker;
				IInputTracker& m_inputTracker;
				::Environment::ITimer& m_timer;

				void UnpinLater(Controls::Knob<TGlyph>& component) const {
					m_timer.Set(::std::chrono::milliseconds{ 400 },
						[this, &component]()->void {
							if (m_inputTracker.get_Track(component)) {
								return;
							}
							States::Knob state{ component.get_State() };
							state.pinned = false;
							component.Set(state, true);
						});
				};
			};
		}
	}
}