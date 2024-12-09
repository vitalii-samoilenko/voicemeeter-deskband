#pragma once

#include "Voicemeeter.UI/Policies/Size/Scales/Stretch.h"

#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Vban : public Glyph<UI::Policies::Size::Scales::Stretch> {
					public:
						Vban(
							Graphics::Palette& palette,
							Trackers::Dirty& dirtyTracker
						);
						Vban() = delete;
						Vban(const Vban&) = delete;
						Vban(Vban&&) = delete;

						~Vban() = default;

						Vban& operator=(const Vban&) = delete;
						Vban& operator=(Vban&&) = delete;

						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};

						virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;

					private:
						::D2D1::ColorF m_color;
					};
				}
			}
		}
	}
}