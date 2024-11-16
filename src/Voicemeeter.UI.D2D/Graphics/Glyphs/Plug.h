#pragma once

#include <string>

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					struct Triangle {};

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