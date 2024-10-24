#include <memory>
#include <utility>

#include <wrl/client.h>

#include "Voicemeeter.DeskBand.UI/Scene.h"
#include "Voicemeeter.DeskBand.UI/Controls/StateControl.h"
#include "Voicemeeter.DeskBand.UI/Policies/CircularStateChangePolicy.h"
#include "Voicemeeter.DeskBand.UI/Policies/PreconfiguredGlyphUpdatePolicy.h"
#include "Voicemeeter.DeskBand.UI/Policies/PreconfiguredInteractivityPolicy.h"
#include "Voicemeeter.DeskBand.UI/Policies/PreconfiguredStatePromotionPolicy.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/Canvas.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/BundleGlyph.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Window.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI;
using namespace ::Voicemeeter::DeskBand::UI::Controls;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;
using namespace ::Voicemeeter::DeskBand::UI::Policies;

void Window::BuildScene() {
	::std::unique_ptr<D2D::Canvas> pCanvas{ new D2D::Canvas{ m_hWnd } };

	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush{ nullptr };
	ThrowIfFailed(pCanvas->get_pRenderTarget()
		->CreateSolidColorBrush(
			::D2D1::ColorF(255.F, 0.F, 0.F, 255.F),
			&pBrush
	), "Brush creation failed");

	auto out_a_1 = [pBrush](const D2D::Canvas& canvas, const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex)->void {
		canvas.get_pRenderTarget()
			->FillRoundedRectangle(
				::D2D1::RoundedRect(::D2D1::RectF(0.F, 0.F, 36.F, 36.F), 2.F, 2.F),
				pBrush.Get());
	};
	using out_a_1_Glyph = D2D::BundleGlyph<decltype(out_a_1)>;
	::std::unique_ptr<out_a_1_Glyph> out_a_1_pGlyph{
		new out_a_1_Glyph{
			*pCanvas,
			{ 36, 36 },
			out_a_1
		} };

	::std::shared_ptr<IStateChangePolicy<int>> pCheckboxStateChangePolicy{
		new CircularStateChangePolicy<int, 0, 1, 1>{}
	};
	auto out_a_1_onPromote = [](const int& state)->void {

	};
	::std::unique_ptr<IStatePromotionPolicy<int>> out_a_1_pStatePromotionPolicy{
		new PreconfiguredStatePromotionPolicy<int, decltype(out_a_1_onPromote)>{ out_a_1_onPromote }
	};
	auto out_a_1_onGlyphUpdate = [](out_a_1_Glyph& glyph, const int& state)->void {

	};
	::std::shared_ptr<IGlyphUpdatePolicy<out_a_1_Glyph, int>> out_a_1_pGlyphUpdatePolicy{
		new PreconfiguredGlyphUpdatePolicy<out_a_1_Glyph, int, decltype(out_a_1_onGlyphUpdate)>{ out_a_1_onGlyphUpdate }
	};
	using out_a_1_StateControl = StateControl<int, out_a_1_Glyph>;
	auto onPointer = [](out_a_1_StateControl& control, const ::linear_algebra::vector& point)->void {

	};
	auto onWheel = [](out_a_1_StateControl& control, const ::linear_algebra::vector& point, int delta) {

	};
	::std::shared_ptr<IInteractivityPolicy<out_a_1_StateControl>> out_a_1_pInteractivityPolicy{
		new PreconfiguredInteractivityPolicy<out_a_1_StateControl, decltype(onPointer), decltype(onWheel)>{
			onPointer, onPointer, onWheel, onPointer, onPointer
		} };

	::std::unique_ptr<out_a_1_StateControl> out_a_1_pStateControl{
		new out_a_1_StateControl{
			{ 2, 2 }, { 2, 2 },
			::std::move(out_a_1_pGlyph),
			pCheckboxStateChangePolicy,
			::std::move(out_a_1_pStatePromotionPolicy),
			out_a_1_pGlyphUpdatePolicy,
			out_a_1_pInteractivityPolicy
		} };

	m_pScene.reset(new Scene{
		*this,
		::std::move(pCanvas),
		::std::move(out_a_1_pStateControl)
	});
}