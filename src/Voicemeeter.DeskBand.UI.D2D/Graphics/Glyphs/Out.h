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
						class Out : public Glyph {
						public:
							Out(
								Graphics::Canvas& canvas,
								const ::std::wstring& label
							) : Glyph{ canvas, { 41, 19 } }
							  , m_label{ label } {

							}
							Out() = delete;
							Out(const Out&) = delete;
							Out(Out&&) = delete;

							~Out() = default;

							Out& operator=(const Out&) = delete;
							Out& operator=(Out&&) = delete;

							virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
								Glyph::Redraw(point, vertex);

								m_canvas.get_pRenderTarget()
									->DrawRoundedRectangle(
										::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
										Brush(),
										1.5F);
								m_canvas.get_pRenderTarget()
									->FillGeometry(
										Triangle(),
										Brush());
								m_canvas.get_pRenderTarget()
									->DrawText(
										m_label.c_str(),
										m_label.size(),
										m_canvas.get_pTextFormat(),
										D2D1::RectF(16.F, 0.5F, 300.F, 200.F),
										Brush());
							};

						private:
							::std::wstring m_label;

							ID2D1SolidColorBrush* Brush() const {
								const ::std::string& key{
									Active
										? "active"
										: "inactive"
								};
								ID2D1SolidColorBrush* pBrush{ nullptr };
								if (!m_canvas.get_pBrush(key, &pBrush)) {
									DWORD color{
										Active
											? RGB(153, 195, 112)
											: RGB(137, 120, 95)
									};
									pBrush->SetColor(::D2D1::ColorF(color));
								}
								return pBrush;
							};

							ID2D1PathGeometry* Triangle() {
								const ::std::string& key{
									"out_triangle"
								};
								ID2D1PathGeometry* pTriangle{ nullptr };
								if (!m_canvas.get_pGeometry(key, &pTriangle)) {
									::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
									ThrowIfFailed(pTriangle->Open(
										&pSink
									), "Geometry initialization failed");

									pSink->BeginFigure(
										::D2D1::Point2F(7.F, 3.5F),
										D2D1_FIGURE_BEGIN_FILLED
									);
									pSink->AddLine(
										::D2D1::Point2F(13.F, 9.F)
									);
									pSink->AddLine(
										::D2D1::Point2F(7.F, 15.5F)
									);
									pSink->EndFigure(
										D2D1_FIGURE_END_CLOSED
									);

									ThrowIfFailed(pSink->Close(
									), "Geometry finalization failed");
								}
								return pTriangle;
							};
						};
					}
				}
			}
		}
	}
}