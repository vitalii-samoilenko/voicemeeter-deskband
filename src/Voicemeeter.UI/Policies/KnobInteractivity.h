#pragma once

#include <cmath>

#include "estd/linear_algebra.h"

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
					IInputTracker& inputTracker
				) : m_inputTracker{ inputTracker } {

				};
				KnobInteractivity() = delete;
				KnobInteractivity(const KnobInteractivity&) = delete;
				KnobInteractivity(KnobInteractivity&&) = delete;

				~KnobInteractivity() = default;

				KnobInteractivity& operator=(const KnobInteractivity&) = delete;
				KnobInteractivity& operator=(KnobInteractivity&&) = delete;

				virtual void MouseLDown(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.EnableInputTrack(component, point);
				};
				virtual void MouseLDouble(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					component.SetDefault();
				};
				virtual void MouseRDown(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					States::Knob state{ component.get_State() };
					state.pinned = !state.pinned;

					component.Set(state, true);
				};
				virtual void MouseRDouble(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					States::Knob state{ component.get_State() };
					state.pinned = !state.pinned;

					component.Set(state, true);
				};
				virtual void MouseWheel(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point, int delta) const override {
					States::Knob state{ component.get_State() };
					state.level += delta * 7;

					component.Set(state, true);
				};
				virtual void MouseMove(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					if (!m_inputTracker.IsTracking(component)) {
						return;
					}

					double scale{ component.get_Size().x / component.get_BaseSize().x };
					States::Knob state{ component.get_State() };
					state.level += static_cast<int>((point.x - m_inputTracker.get_PinPosition().x) * 100. / scale);

					component.Set(state, true);

					m_inputTracker.EnableInputTrack(component, point);
				};
				virtual void MouseLUp(Controls::Knob<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.DisableInputTrack(component);
				};

			private:
				IInputTracker& m_inputTracker;
			};
		}
	}
}