#pragma once

#include <string>

#include "estd/linear_algebra.h"

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					struct Triangle {};

					class Out : public Glyph {
					public:
						Out(
							Graphics::Canvas& canvas,
							const ::std::wstring& label
						);
						Out() = delete;
						Out(const Out&) = delete;
						Out(Out&&) = delete;

						~Out() = default;

						Out& operator=(const Out&) = delete;
						Out& operator=(Out&&) = delete;

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