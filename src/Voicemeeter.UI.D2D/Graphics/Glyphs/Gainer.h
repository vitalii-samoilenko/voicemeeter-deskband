#pragma once

#include "estd/linear_algebra.h"

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Gainer : public Glyph {
					public:
						explicit Gainer(
							Graphics::Canvas& canvas
						);
						Gainer() = delete;
						Gainer(const Gainer&) = delete;
						Gainer(Gainer&&) = delete;

						~Gainer() = default;

						Gainer& operator=(const Gainer&) = delete;
						Gainer& operator=(Gainer&&) = delete;

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;

						inline void set_Level(FLOAT level) {
							m_level = level;
						};

					private:
						FLOAT m_level;
					};
				}
			}
		}
	}
}