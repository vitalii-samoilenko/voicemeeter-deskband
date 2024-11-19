#pragma once

#include <string>

#include <d2d1_3.h>

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Knob : public Glyph {
					public:
						explicit Knob(
							const Graphics::Canvas& canvas
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
						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};
						inline void set_Angle(FLOAT value) {
							m_angle = value;
						};

						virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;

					private:
						::std::wstring m_label;
						::D2D1::ColorF m_color;
						FLOAT m_angle;
					};
				}
			}
		}
	}
}