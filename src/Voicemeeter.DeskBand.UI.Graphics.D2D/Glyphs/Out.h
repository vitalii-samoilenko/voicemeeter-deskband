#pragma once

#include "../Glyph.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					namespace Glyphs {
						template<bool Active>
						class Out final : public Glyph {
						public:
							explicit Out(
								Canvas& canvas,
							) : Glyph{ canvas, { 41, 19 } } {

							}
							Out() = delete;
							Out(const Out&) = delete;
							Out(Out&&) = delete;

							~Out() = default;

							Out& operator=(const Out&) = delete;
							Out& operator=(Out&&) = delete;

							virtual void OnDraw()(const D2D::Canvas& canvas, const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) const override {
								canvas.get_pRenderTarget()
									->FillRectangle(
										::D2D1::RectF(0.F, 0.F, 41.F, 19.F),
										canvas.get_pBackgroundBrush());
								canvas.get_pRenderTarget()
									->FillRoundedRectangle(
										::D2D1::RoundedRect(::D2D1::RectF(0.F, 0.F, 41.F, 19.F), 7.F, 7.F),
										Active
											? canvas.get_pBrush(RGB(112, 195, 153))
											: canvas.get_pBrush(RGB(95, 120, 137)));
							};
						};
					}
				}
			}
		}
	}
}