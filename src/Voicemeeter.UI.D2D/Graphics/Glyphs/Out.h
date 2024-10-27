#pragma once

#include <string>

#include "estd/linear_algebra.h"

#include "Windows/Wrappers.h"

#include <wrl/client.h>
#include <d2d1_3.h>

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					struct Triangle {};

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

							const Palette& palette{ m_canvas.get_Palette() };
							ID2D1SolidColorBrush* pBrush{
								(Active
									? palette.get_pBrush(palette.get_Theme()
										.PrimaryActive)
									: palette.get_pBrush(palette.get_Theme()
										.Inactive))
							};
							bool fresh{ false };
							ID2D1PathGeometry* pTriangle{ palette.get_pGeometry(typeid(Triangle), fresh) };
							if (fresh) {
								::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
								::Windows::ThrowIfFailed(pTriangle->Open(
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

								::Windows::ThrowIfFailed(pSink->Close(
								), "Geometry finalization failed");
							}

							m_canvas.get_pRenderTarget()
								->DrawRoundedRectangle(
									::D2D1::RoundedRect(::D2D1::RectF(0.75F, 0.75F, 40.25F, 18.25F), 6.25F, 6.25F),
									pBrush,
									1.5F);
							m_canvas.get_pRenderTarget()
								->FillGeometry(
									pTriangle,
									pBrush);
							m_canvas.get_pRenderTarget()
								->DrawTextW(
									m_label.c_str(),
									static_cast<UINT32>(m_label.size()),
									palette.get_pTextFormat(palette.get_Theme()
										.FontFamily),
									D2D1::RectF(16.F, 0.5F, 300.F, 200.F),
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