#pragma once

#include "Voicemeeter.UI/Policies/Size/Scales/Stretch.h"

#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Plug : public Glyph<UI::Policies::Size::Scales::Stretch> {
					public:
						Plug(
							Graphics::Palette& palette,
							Trackers::Dirty& dirtyTracker
						);
						Plug() = delete;
						Plug(const Plug&) = delete;
						Plug(Plug&&) = delete;

						~Plug() = default;

						Plug& operator=(const Plug&) = delete;
						Plug& operator=(Plug&&) = delete;

						inline void set_Label(const ::std::wstring& value) {
							m_label = value;
						}
						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};

						virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;

					private:
						::std::wstring m_label;
						::D2D1::ColorF m_color;
					};
				}
			}
		}
	}
}