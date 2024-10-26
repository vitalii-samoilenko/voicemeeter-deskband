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
						template<bool Active>
						class Out : public Glyph {
						public:
							Out(
								Graphics::Canvas& canvas
							) : Glyph{ canvas, { 41, 19 } } {

							}
							Out() = delete;
							Out(const Out&) = delete;
							Out(Out&&) = delete;

							~Out() = default;

							Out& operator=(const Out&) = delete;
							Out& operator=(Out&&) = delete;

							virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
								m_canvas.get_pRenderTarget()
									->FillRoundedRectangle(
										::D2D1::RoundedRect(::D2D1::RectF(0.F, 0.F, 41.F, 19.F), 7.F, 7.F),
										Active
											? m_canvas.get_pBrush(RGB(112, 195, 153))
											: m_canvas.get_pBrush(RGB(95, 120, 137)));
							};
						};
					}
				}
			}
		}
	}
}