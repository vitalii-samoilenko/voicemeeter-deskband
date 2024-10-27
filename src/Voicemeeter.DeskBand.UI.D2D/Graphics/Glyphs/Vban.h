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
						class Vban : public Glyph {
						public:
							Vban(
								Graphics::Canvas& canvas
							) : Glyph{ canvas, { 39, 22 } } { //{65, 22}

							}
							Vban() = delete;
							Vban(const Vban&) = delete;
							Vban(Vban&&) = delete;

							~Vban() = default;

							Vban& operator=(const Vban&) = delete;
							Vban& operator=(Vban&&) = delete;

							virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
								Glyph::Redraw(point, vertex);

								m_canvas.get_pRenderTarget()
									->FillRectangle(
										::D2D1::RectF(0.F, 0.F, 7.5F, 22.F),
										Brush());
								m_canvas.get_pRenderTarget()
									->FillRectangle(
										::D2D1::RectF(31.5F, 0.F, 39.F, 22.F), //57.5
										Brush());
								m_canvas.get_pRenderTarget()
									->DrawRectangle(
										::D2D1::RectF(7.5F, 0.5F, 31.5F, 21.5F),  //57.5
										Brush());
								D2D1_MATRIX_3X2_F base{};
								m_canvas.get_pRenderTarget()
									->GetTransform(&base);
								m_canvas.get_pRenderTarget()
									->SetTransform(::D2D1::Matrix3x2F::Scale(0.8F, 0.8F)
										* base);
								m_canvas.get_pRenderTarget()
									->DrawText(
										L"VBAN",
										1, // 4
										m_canvas.get_pTextFormat(),
										D2D1::RectF(19.F, 5.0F, 300.F, 200.F),
										Brush());
								m_canvas.get_pRenderTarget()
									->SetTransform(base);
							};

						private:
							::std::wstring m_label;

							ID2D1SolidColorBrush* Brush() const {
								const ::std::string& key{
									Active
										? "vban_active"
										: "inactive"
								};
								ID2D1SolidColorBrush* pBrush{ nullptr };
								if (!m_canvas.get_pBrush(key, &pBrush)) {
									DWORD color{
										Active
											? RGB(250, 230, 104)
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