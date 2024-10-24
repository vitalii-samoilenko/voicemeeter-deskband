#include <memory>

#include <wrl/client.h>

#include "Voicemeeter.DeskBand.UI/Scene.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/Canvas.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/BundleGlyph.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Window.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;

void Window::BuildScene() {
	::std::unique_ptr<D2D::Canvas> pCanvas{ new D2D::Canvas{ m_hWnd } };

	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush{ nullptr };
	ThrowIfFailed(pCanvas->get_pRenderTarget()
		->CreateSolidColorBrush(
			::D2D1::ColorF(255.F, 0.F, 0.F, 0.F),
			&pBrush
	), "Brush creation failed");

	auto out_a_1 = [pBrush](const D2D::Canvas& canvas, const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex)->void {
		canvas.get_pRenderTarget()
			->FillRoundedRectangle(
				::D2D1::RoundedRect(::D2D1::RectF(0.F, 0.F, 36.F, 36.F), 6.F, 6.F),
				pBrush.Get());
	};
	//::std::unique_ptr<D2D::BundleGlyph<decltype(out_a_1)>> pGlyph{ new D2D::BundleGlyph<decltype(out_a_1)>{
	//	*pCanvas,
	//	{ 36, 36 },
	//	out_a_1
	//}};
}