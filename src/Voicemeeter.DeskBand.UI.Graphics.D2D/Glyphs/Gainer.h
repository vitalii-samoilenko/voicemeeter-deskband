#pragma once

#include "../Glyph.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					namespace Glyphs {
						class Gainer final : public Glyph {
						public:
							explicit Gainer(
								Canvas& canvas
							);
							Gainer() = delete;
							Gainer(const Gainer&) = delete;
							Gainer(Gainer&&) = delete;

							~Gainer() = default;

							Gainer& operator=(const Gainer&) = delete;
							Gainer& operator=(Gainer&&) = delete;

							virtual void OnDraw(const D2D::Canvas& canvas, const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) const override;

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
}