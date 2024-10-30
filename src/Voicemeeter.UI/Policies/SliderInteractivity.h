#pragma once

#include "estd/linear_algebra.h"

#include "../Graphics/IGlyph.h"
#include "../IInputTracker.h"
#include "../Controls/Slider.h"
#include "IInteractivity.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TGlyph>
			class SliderInteractivity : public IInteractivity<Controls::Slider<TGlyph>> {
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

				virtual void MouseLDown(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.set_Track(component, true);
					m_inputTracker.set_Position(point);
				};
				virtual void MouseLDouble(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point) const override {
					component.SetDefault();
				};
				virtual void MouseRDown(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};
				virtual void MouseRDouble(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};
				virtual void MouseWheel(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point, int delta) const override {
					int level{ component.get_State() + delta * 5 };

					component.Set(level, true);
				};
				virtual void MouseMove(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point) const override {
					if (!m_inputTracker.get_Track(component)) {
						return;
					}

					double scale{ component.get_Size().x / component.get_BaseSize().x };
					int level{ component.get_State() + static_cast<int>((point.x - m_inputTracker.get_Position().x) * 100. / scale) };

					component.Set(level, true);

					m_inputTracker.set_Position(point);
				};
				virtual void MouseLUp(Controls::Slider<Graphics::IGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_inputTracker.set_Track(component, false);
				};

			private:
				IInputTracker& m_inputTracker;
			};
		}
	}
}