#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Bundles {
					class Knob : public Bundle {
					public:
						Knob(
							Graphics::Palette& palette,
							size_t label
						);
						Knob() = delete;
						Knob(const Knob&) = delete;
						Knob(Knob&&) = delete;

						~Knob() = default;

						Knob& operator=(const Knob&) = delete;
						Knob& operator=(Knob&&) = delete;

						inline void set_Label(size_t value) {
							m_label = value;
						};
						inline void set_FrameColor(const ::D2D1::ColorF& value) {
							m_frameColor = value;
						};
						inline void set_LabelColor(const ::D2D1::ColorF& value) {
							m_labelColor = value;
						};
						inline void set_Angle(double value) {
							m_angle = value;
						};

						virtual void Execute() override;

					private:
						size_t m_label;
						::D2D1::ColorF m_frameColor;
						::D2D1::ColorF m_labelColor;
						double m_angle;
					};
				}
			}
		}
	}
}