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
					class Plug : public Glyph {
					public:
						Plug(
							Graphics::Canvas& canvas,
							const ::std::wstring& label
						);
						Plug() = delete;
						Plug(const Plug&) = delete;
						Plug(Plug&&) = delete;

						~Plug() = default;

						Plug& operator=(const Plug&) = delete;
						Plug& operator=(Plug&&) = delete;

						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};

						virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;

					private:
						const ::std::wstring m_label;
						::D2D1::ColorF m_color;
					};
				}
			}
		}
	}
}