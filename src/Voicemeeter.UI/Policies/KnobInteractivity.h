#pragma once

#include <cmath>

#include "estd/linear_algebra.h"

#include "Environment/ITimer.h"

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
					::Environment::ITimer& timer
				) : m_inputTracker{ inputTracker }
				  , m_timer{ timer } {

				};
				KnobInteractivity() = delete;
				KnobInteractivity(const KnobInteractivity&) = delete;
				KnobInteractivity(KnobInteractivity&&) = delete;

				~KnobInteractivity() = default;

				KnobInteractivity& operator=(const KnobInteractivity&) = delete;
				KnobInteractivity& operator=(KnobInteractivity&&) = delete;

				virtual void MouseLDown(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.EnableInputTrack(component, point);

					States::Knob state{ component.get_State() };
					if (!state.pinned) {
						m_timer.Elapse();
						state.pinned = true;
					}

					component.Set(state, true);
				};
				virtual void MouseLDouble(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					component.SetDefault();

					UnpinLater(component);
				};
				virtual void MouseRDown(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					States::Knob state{ component.get_State() };
					state.enabled = !state.enabled;
					if (!state.pinned) {
						m_timer.Elapse();
					}

					component.Set(state, true);
				};
				virtual void MouseRDouble(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					States::Knob state{ component.get_State() };
					state.enabled = !state.enabled;

					component.Set(state, true);
				};
				virtual void MouseWheel(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point, int delta) const override {
					States::Knob state{ component.get_State() };
					state.gain += delta * 7;
					if (!state.pinned) {
						m_timer.Elapse();
						state.pinned = true;
					}

					component.Set(state, true);

					UnpinLater(component);
				};
				virtual void MouseMove(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					if (!m_inputTracker.IsTracking(component)) {
						return;
					}

					double scale{ component.get_Size().x / component.get_BaseSize().x };
					States::Knob state{ component.get_State() };
					state.gain += static_cast<int>((point.x - m_inputTracker.get_PinPosition().x) * 100. / scale);

					component.Set(state, true);

					m_inputTracker.EnableInputTrack(component, point);
				};
				virtual void MouseLUp(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.DisableInputTrack(component);

					UnpinLater(component);
				};

			private:
				IInputTracker& m_inputTracker;
				::Environment::ITimer& m_timer;

				void UnpinLater(Controls::Knob<TGlyph>& component) const {
					m_timer.Set(::std::chrono::milliseconds{ 400 },
						[this, &component]()->void {
							if (m_inputTracker.IsTracking(component)) {
								return;
							}

							States::Knob state{ component.get_State() };
							state.pinned = false;

							component.Set(state, true);
						});
				}
			};
		}
	}
}