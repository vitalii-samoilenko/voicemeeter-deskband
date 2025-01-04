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
							if (m_label == value) {
								return;
							}
							m_label = value;
							m_changed.set(label);
						};
						inline void set_FrameColor(const ::D2D1::ColorF& value) {
							m_frameColor = value;
						};
						inline void set_LabelColor(const ::D2D1::ColorF& value) {
							m_labelColor = value;
						};
						inline void set_Angle(int value) {
							if (m_angle == value) {
								return;
							}
							m_angle = value;
							m_changed.set(angle);
						};

						virtual void Execute() override;

					private:
						enum knob_flags : size_t {
							label = size + 1,
							angle = size + 2
						};

						size_t m_label;
						::D2D1::ColorF m_frameColor;
						::D2D1::ColorF m_labelColor;
						int m_angle;
						::std::valarray<double> m_maskPoint;
						::std::valarray<double> m_labelPoint;
						::std::valarray<double> m_labelVertex;
						::std::valarray<double> m_labelMaskPoint;
						::std::valarray<double> m_indicatorPoint;
						::std::valarray<double> m_indicatorVertex;
						::std::valarray<double> m_indicatorMaskPoint;
					};
				}
			}
		}
	}
}