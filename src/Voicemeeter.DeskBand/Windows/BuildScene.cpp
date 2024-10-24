#include <memory>
#include <utility>

#include <wrl/client.h>

#include "Voicemeeter.DeskBand.UI/Scene.h"
#include "Voicemeeter.DeskBand.UI/Controls/StateControl.h"
#include "Voicemeeter.DeskBand.UI/Panels/StackPanel.h"
#include "Voicemeeter.DeskBand.UI/Policies/CircularStateChangePolicy.h"
#include "Voicemeeter.DeskBand.UI/Policies/PreconfiguredGlyphUpdatePolicy.h"
#include "Voicemeeter.DeskBand.UI/Policies/PreconfiguredInteractivityPolicy.h"
#include "Voicemeeter.DeskBand.UI/Policies/PreconfiguredStatePromotionPolicy.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/BundleGlyph.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/Canvas.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/FrameGlyph.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Window.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI;
using namespace ::Voicemeeter::DeskBand::UI::Controls;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;
using namespace ::Voicemeeter::DeskBand::UI::Panels;
using namespace ::Voicemeeter::DeskBand::UI::Policies;

class OutBundle final {
public:
	OutBundle(
		const ::D2D::Canvas& canvas,
		const ::D2D1::ColorF& color
	) : pBrush{ nullptr } {
		ThrowIfFailed(canvas.get_pRenderTarget()
			->CreateSolidColorBrush(
				color,
				&pBrush
		), "Brush creation failed");
	}
	OutBundle() = delete;
	OutBundle(const OutBundle&) = delete;
	OutBundle(OutBundle&&) = default;

	~OutBundle() = default;

	OutBundle& operator=(const OutBundle&) = delete;
	OutBundle& operator=(OutBundle&&) = default;

	void operator()(const D2D::Canvas& canvas, const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) const {
		canvas.get_pRenderTarget()
			->FillRoundedRectangle(
				::D2D1::RoundedRect(::D2D1::RectF(0.F, 0.F, 41.F, 19.F), 13.F, 13.F),
				pBrush.Get());
	}

private:
	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
};

using Checkbox = StateControl<int, D2D::FrameGlyph>;

class CheckboxOnMouseLDown final {
public:
	CheckboxOnMouseLDown() = default;
	CheckboxOnMouseLDown(const CheckboxOnMouseLDown&) = delete;
	CheckboxOnMouseLDown(CheckboxOnMouseLDown&&) = default;

	~CheckboxOnMouseLDown() = default;

	CheckboxOnMouseLDown& operator=(const CheckboxOnMouseLDown&) = delete;
	CheckboxOnMouseLDown& operator=(CheckboxOnMouseLDown&&) = default;

	void operator()(Checkbox& control, const ::linear_algebra::vector& point) const {
		control.SetNext();
	}
};

class CheckboxOnMouseIgnore final {
public:
	CheckboxOnMouseIgnore() = default;
	CheckboxOnMouseIgnore(const CheckboxOnMouseIgnore&) = delete;
	CheckboxOnMouseIgnore(CheckboxOnMouseIgnore&&) = default;

	~CheckboxOnMouseIgnore() = default;

	CheckboxOnMouseIgnore& operator=(const CheckboxOnMouseIgnore&) = delete;
	CheckboxOnMouseIgnore& operator=(CheckboxOnMouseIgnore&&) = default;

	void operator()(Checkbox& control, const ::linear_algebra::vector& point) const {

	}
};

class CheckboxOnWheelIgnore final {
public:
	CheckboxOnWheelIgnore() = default;
	CheckboxOnWheelIgnore(const CheckboxOnWheelIgnore&) = delete;
	CheckboxOnWheelIgnore(CheckboxOnWheelIgnore&&) = default;

	~CheckboxOnWheelIgnore() = default;

	CheckboxOnWheelIgnore& operator=(const CheckboxOnWheelIgnore&) = delete;
	CheckboxOnWheelIgnore& operator=(CheckboxOnWheelIgnore&&) = default;

	void operator()(Checkbox& control, const ::linear_algebra::vector& point, int delta) const {

	}
};

using CheckboxStateChangePolicy = CircularStateChangePolicy<int, 0, 1, 1>;

void Window::BuildScene() {
	const ::D2D1::ColorF inactive{ ::D2D1::ColorF(95 / 255.F, 120 / 255.F, 137 / 255.F, 1.F) };
	const ::D2D1::ColorF active{ ::D2D1::ColorF(112 / 255.F, 195 / 255.F, 153 / 255.F, 1.F) };

	::std::unique_ptr<D2D::Canvas> pCanvas{ new D2D::Canvas{ m_hWnd } };

	::std::unique_ptr<IGlyph> out_a_1_cpFrame[]{
		::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
			new D2D::BundleGlyph<OutBundle>{
				*pCanvas,
				{ 41, 19 },
				OutBundle{ *pCanvas, inactive }
			}
		}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
			new D2D::BundleGlyph<OutBundle>{
				*pCanvas,
				{ 41, 19 },
				OutBundle{ *pCanvas, active }
			}
	} };
	::std::unique_ptr<D2D::FrameGlyph> out_a_1_pGlyph{
		new D2D::FrameGlyph{
			::std::begin(out_a_1_cpFrame),
			::std::end(out_a_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_a_2_cpFrame[]{
	::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ *pCanvas, inactive }
		}
	}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ *pCanvas, active }
		}
	} };
	::std::unique_ptr<D2D::FrameGlyph> out_a_2_pGlyph{
		new D2D::FrameGlyph{
			::std::begin(out_a_2_cpFrame),
			::std::end(out_a_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_b_1_cpFrame[]{
	::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ *pCanvas, inactive }
		}
	}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ *pCanvas, active }
		}
	} };
	::std::unique_ptr<D2D::FrameGlyph> out_b_1_pGlyph{
		new D2D::FrameGlyph{
			::std::begin(out_b_1_cpFrame),
			::std::end(out_b_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_b_2_cpFrame[]{
	::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ *pCanvas, inactive }
		}
	}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>> {
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ *pCanvas, active }
		}
	} };
	::std::unique_ptr<D2D::FrameGlyph> out_b_2_pGlyph{
		new D2D::FrameGlyph{
			::std::begin(out_b_2_cpFrame),
			::std::end(out_b_2_cpFrame)
	} };

	::std::shared_ptr<CheckboxStateChangePolicy> pCheckboxStateChangePolicy{
		new CheckboxStateChangePolicy{}
	};
	auto out_a_1_onPromote = [](const int& state)->void {

	};
	using out_a_1_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_a_1_onPromote)>;
	::std::unique_ptr<out_a_1_StatePromotionPolicy> out_a_1_pStatePromotionPolicy{
		new out_a_1_StatePromotionPolicy{ out_a_1_onPromote }
	};
	auto out_a_2_onPromote = [](const int& state)->void {

	};
	using out_a_2_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_a_2_onPromote)>;
	::std::unique_ptr<out_a_2_StatePromotionPolicy> out_a_2_pStatePromotionPolicy{
		new out_a_2_StatePromotionPolicy{ out_a_2_onPromote }
	};
	auto out_b_1_onPromote = [](const int& state)->void {

	};
	using out_b_1_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_b_1_onPromote)>;
	::std::unique_ptr<out_b_1_StatePromotionPolicy> out_b_1_pStatePromotionPolicy{
		new out_b_1_StatePromotionPolicy{ out_b_1_onPromote }
	};
	auto out_b_2_onPromote = [](const int& state)->void {

	};
	using out_b_2_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_b_2_onPromote)>;
	::std::unique_ptr<out_b_2_StatePromotionPolicy> out_b_2_pStatePromotionPolicy{
		new out_b_2_StatePromotionPolicy{ out_b_2_onPromote }
	};
	auto onFrameGlyphUpdate = [](D2D::FrameGlyph& glyph, const int& state)->void {
		glyph.set_Frame(state);
	};
	using FrameGlyphUpdatePolicy = PreconfiguredGlyphUpdatePolicy<D2D::FrameGlyph, int, decltype(onFrameGlyphUpdate)>;
	::std::shared_ptr<FrameGlyphUpdatePolicy> pFrameGlyphUpdatePolicy{
		new FrameGlyphUpdatePolicy{ onFrameGlyphUpdate }
	};
	using CheckboxInteractivityPolicy = PreconfiguredInteractivityPolicy<Checkbox,
		CheckboxOnMouseLDown,
		CheckboxOnMouseIgnore,
		CheckboxOnWheelIgnore,
		CheckboxOnMouseIgnore,
		CheckboxOnMouseIgnore>;
	::std::shared_ptr<CheckboxInteractivityPolicy> pCheckboxInteractivityPolicy{
		new CheckboxInteractivityPolicy{
			CheckboxOnMouseLDown{},
			CheckboxOnMouseIgnore{},
			CheckboxOnWheelIgnore{},
			CheckboxOnMouseIgnore{},
			CheckboxOnMouseIgnore{}
	} };

	::std::unique_ptr<IComponent> out_a_cpControl[]{
		::std::unique_ptr<Checkbox>{
			new Checkbox{
				{ 0, 0 }, { 0, 0 },
				::std::move(out_a_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(out_a_1_pStatePromotionPolicy),
				pFrameGlyphUpdatePolicy,
				pCheckboxInteractivityPolicy
		} }, ::std::unique_ptr<Checkbox>{
			new Checkbox{
				{ 0, 2 }, { 0, 0 },
				::std::move(out_a_2_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(out_a_2_pStatePromotionPolicy),
				pFrameGlyphUpdatePolicy,
				pCheckboxInteractivityPolicy
		} }
	};

	::std::unique_ptr<StackPanel<Direction::Down>> out_a_pPanel{
		new StackPanel<Direction::Down>(
			::linear_algebra::vector{ 0, 0 },
			::linear_algebra::vector{ 0, 0 },
			::std::begin(out_a_cpControl),
			::std::end(out_a_cpControl)
	) };

	m_pScene.reset(new Scene{
		*this,
		::std::move(pCanvas),
		::std::move(out_a_pPanel)
	});
}