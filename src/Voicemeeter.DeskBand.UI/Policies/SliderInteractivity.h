#pragma once

#include "estd/linear_algebra.h"

#include "../IInputTracker.h"
#include "../Controls/Slider.h"
#include "IInteractivity.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TGlyph>
				class SliderInteractivity final : public IInteractivity<Controls::Slider<TGlyph>> {
				public:
					explicit SliderInteractivity(
						IInputTracker& inputTracker
					) : m_inputTracker{ inputTracker } {

					};
					SliderInteractivity() = delete;
					SliderInteractivity(const SliderInteractivity&) = delete;
					SliderInteractivity(SliderInteractivity&&) = delete;

					~SliderInteractivity() = default;

					SliderInteractivity& operator=(const SliderInteractivity&) = delete;
					SliderInteractivity& operator=(SliderInteractivity&&) = delete;

					virtual void MouseLDown(Controls::Slider<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
						m_inputTracker.EnableInputTrack(component);
					};
					virtual void MouseLDouble(Controls::Slider<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
						component.SetDefault();
					};
					virtual void MouseRDown(Controls::Slider<TGlyph>& component, const ::linear_algebra::vectord& point) const override {

					};
					virtual void MouseWheel(Controls::Slider<TGlyph>& component, const ::linear_algebra::vectord& point, int delta) const override {
						int level{ component.get_State() + delta * 5 };

						component.Set(level, true);
					};
					virtual void MouseMove(Controls::Slider<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
						if (!m_inputTracker.IsTracking(component)) {
							return;
						}

						double scale{ component.get_Size().x / component.get_BaseSize().x };
						int level{ static_cast<int>((point.x - component.get_Position().x) / scale) * 100 };

						component.Set(level, true);
					};
					virtual void MouseLUp(Controls::Slider<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
						m_inputTracker.DisableInputTrack(component);
					};

				private:
					IInputTracker& m_inputTracker;
				};
			}
		}
	}
}