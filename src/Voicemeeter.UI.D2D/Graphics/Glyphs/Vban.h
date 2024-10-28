#pragma once

#include <string>

#include "estd/linear_algebra.h"

#include "Windows/Wrappers.h"

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					template<bool Active>
					class Vban : public Glyph {
					public:
						explicit Vban(
							Graphics::Canvas& canvas
						) : Glyph{ canvas, { 39, 22 } }
						  , m_label{ L"V" } {

						}
						Vban() = delete;
						Vban(const Vban&) = delete;
						Vban(Vban&&) = delete;

						~Vban() = default;

						Vban& operator=(const Vban&) = delete;
						Vban& operator=(Vban&&) = delete;

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
							Glyph::Redraw(point, vertex);

							const Palette& palette{ m_canvas.get_Palette() };
							ID2D1SolidColorBrush* pBrush{
								(Active
									? palette.get_pBrush(palette.get_Theme()
										.SecondaryActive)
									: palette.get_pBrush(palette.get_Theme()
										.Inactive))
							};
							IDWriteTextLayout* pLayout{
								palette.get_pTextLayout(
									m_label,
									palette.get_Theme()
										.FontFamily
								)
							};
							DWRITE_TEXT_METRICS metrics{};
							::Windows::ThrowIfFailed(pLayout->GetMetrics(
								&metrics
							), "Text measurement failed");

							m_canvas.get_pRenderTarget()
								->FillRectangle(
									::D2D1::RectF(0.F, 0.F, 7.5F, 22.F),
									pBrush);
							m_canvas.get_pRenderTarget()
								->FillRectangle(
									::D2D1::RectF(31.5F, 0.F, 39.F, 22.F),
									pBrush);
							m_canvas.get_pRenderTarget()
								->DrawRectangle(
									::D2D1::RectF(7.5F, 0.5F, 31.5F, 21.5F),
									pBrush);
							m_canvas.get_pRenderTarget()
								->DrawTextLayout(
									::D2D1::Point2F((39.F - metrics.width) / 2, (22.F - metrics.height) / 2),
									pLayout,
									pBrush);
						};

					private:
						::std::wstring m_label;
					};
				}
			}
		}
	}
}