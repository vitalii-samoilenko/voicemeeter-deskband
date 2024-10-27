#pragma once

#include <string>

#include "estd/linear_algebra.h"

#include <wrl/client.h>
#include <d2d1_3.h>

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "../Canvas.h"
#include "../Glyph.h"

using namespace ::Voicemeeter::DeskBand::Windows;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Graphics {
					namespace Glyphs {
						template<bool Active>
						class Mute : public Glyph {
						public:
							explicit Mute(
								Graphics::Canvas& canvas
							) : Glyph{ canvas, { 41, 40 } } {

							}
							Mute() = delete;
							Mute(const Mute&) = delete;
							Mute(Mute&&) = delete;

							~Mute() = default;

							Mute& operator=(const Mute&) = delete;
							Mute& operator=(Mute&&) = delete;

							virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
								Glyph::Redraw(point, vertex);

								const Palette& palette{ m_canvas.get_Palette() };
								ID2D1SolidColorBrush* pBrush{
									(Active
										? palette.get_pBrush(palette.get_Theme()
											.Danger)
										: palette.get_pBrush(palette.get_Theme()
											.Inactive))
								};

								m_canvas.get_pRenderTarget()
									->DrawRoundedRectangle(
										::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 39.25F), 6.25F, 6.25F),
										pBrush,
										1.5F);
								D2D1_MATRIX_3X2_F base{};
								m_canvas.get_pRenderTarget()
									->GetTransform(&base);
								m_canvas.get_pRenderTarget()
									->SetTransform(::D2D1::Matrix3x2F::Scale(1.5F, 1.5F)
										* base);
								m_canvas.get_pRenderTarget()
									->DrawText(
										L"M",
										1,
										palette.get_pTextFormat(palette.get_Theme()
											.FontFamily),
										D2D1::RectF(7.F, 4.0F, 300.F, 200.F),
										pBrush);
								m_canvas.get_pRenderTarget()
									->SetTransform(base);
							};

						private:
							::std::wstring m_label;
						};
					}
				}
			}
		}
	}
}