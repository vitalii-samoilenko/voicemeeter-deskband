#pragma once

#include <string>

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Vban : public Glyph {
					public:
						explicit Vban(
							Graphics::Canvas& canvas
						);
						Vban() = delete;
						Vban(const Vban&) = delete;
						Vban(Vban&&) = delete;

						~Vban() = default;

						Vban& operator=(const Vban&) = delete;
						Vban& operator=(Vban&&) = delete;

						inline void set_Active(bool value) {
							m_active = value;
						};

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;

					private:
						::std::wstring m_label;
						bool m_active;
					};
				}
			}
		}
	}
}