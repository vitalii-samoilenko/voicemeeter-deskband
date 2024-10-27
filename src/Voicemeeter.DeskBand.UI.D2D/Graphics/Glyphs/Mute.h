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
							Mute(
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

								m_canvas.get_pRenderTarget()
									->DrawRoundedRectangle(
										::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 39.25F), 6.25F, 6.25F),
										Brush(),
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
										m_canvas.get_pTextFormat(),
										D2D1::RectF(7.F, 4.0F, 300.F, 200.F),
										Brush());
								m_canvas.get_pRenderTarget()
									->SetTransform(base);
							};

						private:
							::std::wstring m_label;

							ID2D1SolidColorBrush* Brush() const {
								const ::std::string& key{
									Active
										? "danger"
										: "inactive"
								};
								ID2D1SolidColorBrush* pBrush{ nullptr };
								if (!m_canvas.get_pBrush(key, &pBrush)) {
									DWORD color{
										Active
											? RGB(77, 99, 248)
											: RGB(137, 120, 95)
									};
									pBrush->SetColor(::D2D1::ColorF(color));
								}
								return pBrush;
							};
						};
					}
				}
			}
		}
	}
}