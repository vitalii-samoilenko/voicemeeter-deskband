#pragma once

#include "Voicemeeter.UI/Policies/Size/Scales/Stretch.h"

#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Knob : public Glyph<UI::Policies::Size::Scales::Stretch> {
					public:
						Knob(
							Graphics::Palette& palette,
							Trackers::Dirty& dirtyTracker
						);
						Knob() = delete;
						Knob(const Knob&) = delete;
						Knob(Knob&&) = delete;

						~Knob() = default;

						Knob& operator=(const Knob&) = delete;
						Knob& operator=(Knob&&) = delete;

						inline void set_Label(const ::std::wstring& value) {
							m_label = value;
						};
						inline void set_FrameColor(const ::D2D1::ColorF& value) {
							m_frameColor = value;
						};
						inline void set_LabelColor(const ::D2D1::ColorF& value) {
							m_labelColor = value;
						};
						inline void set_Angle(FLOAT value) {
							m_angle = value;
						};

						virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;

					private:
						::std::wstring m_label;
						::D2D1::ColorF m_frameColor;
						::D2D1::ColorF m_labelColor;
						FLOAT m_angle;
					};
				}
			}
		}
	}
}