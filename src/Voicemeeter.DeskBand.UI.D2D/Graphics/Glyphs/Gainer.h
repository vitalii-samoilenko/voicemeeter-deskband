#pragma once

#include "estd/linear_algebra.h"

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Graphics {
					namespace Glyphs {
						class Gainer : public Glyph {
						public:
							Gainer(
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

							ID2D1SolidColorBrush* Brush() const;
							ID2D1SolidColorBrush* Light() const;
							ID2D1SolidColorBrush* Dark() const;
							ID2D1SolidColorBrush* White() const;
							ID2D1PathGeometry* HalfCircle() const;
						};
					}
				}
			}
		}
	}
}