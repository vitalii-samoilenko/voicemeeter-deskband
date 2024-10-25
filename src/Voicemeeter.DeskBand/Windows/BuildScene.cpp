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
#include "Voicemeeter.DeskBand.UI/Policies/RangeStateChangePolicy.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/BundleGlyph.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/Canvas.h"
#include "Voicemeeter.DeskBand.UI.Graphics.D2D/FrameGlyph.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "DeskBand.h"

using namespace ::Voicemeeter::DeskBand::Windows;
using namespace ::Voicemeeter::DeskBand::UI;
using namespace ::Voicemeeter::DeskBand::UI::Controls;
using namespace ::Voicemeeter::DeskBand::UI::Graphics;
using namespace ::Voicemeeter::DeskBand::UI::Panels;
using namespace ::Voicemeeter::DeskBand::UI::Policies;

class OutBundle final {
public:
	explicit OutBundle(
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush
	) : m_pBrush{ pBrush } {

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
				::D2D1::RoundedRect(::D2D1::RectF(0.F, 0.F, 41.F, 19.F), 7.F, 7.F),
				m_pBrush.Get());
	};

private:
	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
};

class GainerBundle final {
public:
	explicit GainerBundle(
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush
	) :	m_pBrush{ pBrush }
	  , m_level{ 0.F } {

	}
	GainerBundle() = delete;
	GainerBundle(const GainerBundle&) = delete;
	GainerBundle(GainerBundle&&) = default;

	~GainerBundle() = default;

	GainerBundle& operator=(const GainerBundle&) = delete;
	GainerBundle& operator=(GainerBundle&&) = default;

	void operator()(const D2D::Canvas& canvas, const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) const {
		canvas.get_pRenderTarget()
			->FillRectangle(
				::D2D1::RectF(0.F, 0.F, 165.F, 40.F),
				canvas.get_pBackgroundBrush());
		canvas.get_pRenderTarget()
			->FillRoundedRectangle(
				::D2D1::RoundedRect(::D2D1::RectF(0.F, 11.F, 165.F, 29.F), 8.F, 8.F),
				m_pBrush.Get());
		canvas.get_pRenderTarget()
			->FillEllipse(
				::D2D1::Ellipse(::D2D1::Point2F(28.F + m_level * 1.5F, 20.F), 20.F, 20.F),
				+m_pBrush.Get());
	};

	void set_Level(FLOAT level) const {
		m_level = level;
	};

private:
	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
	mutable FLOAT m_level;
};

using Carousel = StateControl<int, D2D::FrameGlyph>;
using Gainer = StateControl<int, D2D::BundleGlyph<GainerBundle>>;

class CarouselOnMouseLDown final {
public:
	CarouselOnMouseLDown() = default;
	CarouselOnMouseLDown(const CarouselOnMouseLDown&) = delete;
	CarouselOnMouseLDown(CarouselOnMouseLDown&&) = default;

	~CarouselOnMouseLDown() = default;

	CarouselOnMouseLDown& operator=(const CarouselOnMouseLDown&) = delete;
	CarouselOnMouseLDown& operator=(CarouselOnMouseLDown&&) = default;

	void operator()(Carousel& control, const ::linear_algebra::vector& point) const {
		control.SetNext();
	};
};

class GainerOnMouseLDown final {
public:
	explicit GainerOnMouseLDown(
		IMouseTracker& mouseTracker
	) : m_mouseTracker{ mouseTracker } {

	};
	GainerOnMouseLDown() = delete;
	GainerOnMouseLDown(const GainerOnMouseLDown&) = delete;
	GainerOnMouseLDown(GainerOnMouseLDown&&) = default;

	~GainerOnMouseLDown() = default;

	GainerOnMouseLDown& operator=(const GainerOnMouseLDown&) = delete;
	GainerOnMouseLDown& operator=(GainerOnMouseLDown&&) = default;

	void operator()(Gainer& control, const ::linear_algebra::vector& point) const {
		m_mouseTracker.EnableMouseTrack(control);
	};

private:
	IMouseTracker& m_mouseTracker;
};

class GainerOnMouseWheel final {
public:
	GainerOnMouseWheel() = default;
	GainerOnMouseWheel(const GainerOnMouseWheel&) = delete;
	GainerOnMouseWheel(GainerOnMouseWheel&&) = default;

	~GainerOnMouseWheel() = default;

	GainerOnMouseWheel& operator=(const GainerOnMouseWheel&) = delete;
	GainerOnMouseWheel& operator=(GainerOnMouseWheel&&) = default;

	void operator()(Gainer& control, const ::linear_algebra::vector& point, int delta) const {
		int level{ control.get_State() + delta / 4 };

		control.Set(level, true);
	};
};

class GainerOnMouseMove final {
public:
	GainerOnMouseMove() = default;
	GainerOnMouseMove(const GainerOnMouseMove&) = delete;
	GainerOnMouseMove(GainerOnMouseMove&&) = default;

	~GainerOnMouseMove() = default;

	GainerOnMouseMove& operator=(const GainerOnMouseMove&) = delete;
	GainerOnMouseMove& operator=(GainerOnMouseMove&&) = default;

	void operator()(Gainer& control, const ::linear_algebra::vector& point) const {
		double scale{ static_cast<double>(control.get_Size().x) / control.get_BaseSize().x };
		int level{ static_cast<int>((point.x - (28 * scale) - control.get_Position().x) * 10 / (1.5 * scale)) - 600  };

		control.Set(level, true);
	};
};

class GainerOnMouseLUp final {
public:
	explicit GainerOnMouseLUp(
		IMouseTracker& mouseTracker
	) : m_mouseTracker{ mouseTracker } {

	};
	GainerOnMouseLUp() = delete;
	GainerOnMouseLUp(const GainerOnMouseLUp&) = delete;
	GainerOnMouseLUp(GainerOnMouseLUp&&) = default;

	~GainerOnMouseLUp() = default;

	GainerOnMouseLUp& operator=(const GainerOnMouseLUp&) = delete;
	GainerOnMouseLUp& operator=(GainerOnMouseLUp&&) = default;

	void operator()(Gainer& control, const ::linear_algebra::vector& point) const {
		m_mouseTracker.DisableMouseTrack(control);
	};

private:
	IMouseTracker& m_mouseTracker;
};

class OnMouseIgnore final {
public:
	OnMouseIgnore() = default;
	OnMouseIgnore(const OnMouseIgnore&) = delete;
	OnMouseIgnore(OnMouseIgnore&&) = default;

	~OnMouseIgnore() = default;

	OnMouseIgnore& operator=(const OnMouseIgnore&) = delete;
	OnMouseIgnore& operator=(OnMouseIgnore&&) = default;

	void operator()(Control& control, const ::linear_algebra::vector& point) const {

	};
};

class OnMouseWheelIgnore final {
public:
	OnMouseWheelIgnore() = default;
	OnMouseWheelIgnore(const OnMouseWheelIgnore&) = delete;
	OnMouseWheelIgnore(OnMouseWheelIgnore&&) = default;

	~OnMouseWheelIgnore() = default;

	OnMouseWheelIgnore& operator=(const OnMouseWheelIgnore&) = delete;
	OnMouseWheelIgnore& operator=(OnMouseWheelIgnore&&) = default;

	void operator()(Carousel& control, const ::linear_algebra::vector& point, int delta) const {

	};
};

using OutStateChangePolicy = CircularStateChangePolicy<int, 0, 1, 1>;
using GainerStateChangePolicy = RangeStateChangePolicy<int, -600, 120, 1>;

void DeskBand::BuildScene() {
	::std::unique_ptr<Scene> pScene{ new Scene{ *this } };

	const ::D2D1::ColorF background{ ::D2D1::ColorF(44 / 255.F, 61 / 255.F, 77 / 255.F, 1.F) };
	const ::D2D1::ColorF inactive{ ::D2D1::ColorF(95 / 255.F, 120 / 255.F, 137 / 255.F, 1.F) };
	const ::D2D1::ColorF active{ ::D2D1::ColorF(112 / 255.F, 195 / 255.F, 153 / 255.F, 1.F) };

	::std::unique_ptr<D2D::Canvas> pCanvas{ new D2D::Canvas{ m_hWnd, background } };

	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pInactiveBrush{ nullptr };
	ThrowIfFailed(pCanvas
		->get_pRenderTarget()
		->CreateSolidColorBrush(
			inactive,
			&pInactiveBrush
		), "Brush creation failed");

	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pActiveBrush{ nullptr };
	ThrowIfFailed(pCanvas
		->get_pRenderTarget()
		->CreateSolidColorBrush(
			active,
			&pActiveBrush
		), "Brush creation failed");

	::std::unique_ptr<IGlyph> out_a_1_cpFrame[]{
		::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
			new D2D::BundleGlyph<OutBundle>{
				*pCanvas,
				{ 41, 19 },
				OutBundle{ pInactiveBrush }
			}
		}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
			new D2D::BundleGlyph<OutBundle>{
				*pCanvas,
				{ 41, 19 },
				OutBundle{ pActiveBrush }
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
			OutBundle{ pInactiveBrush }
		}
	}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ pActiveBrush }
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
			OutBundle{ pInactiveBrush }
		}
	}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>>{
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ pActiveBrush }
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
			OutBundle{ pInactiveBrush }
		}
	}, ::std::unique_ptr<D2D::BundleGlyph<OutBundle>> {
		new D2D::BundleGlyph<OutBundle>{
			*pCanvas,
			{ 41, 19 },
			OutBundle{ pActiveBrush }
		}
	} };
	::std::unique_ptr<D2D::FrameGlyph> out_b_2_pGlyph{
		new D2D::FrameGlyph{
			::std::begin(out_b_2_cpFrame),
			::std::end(out_b_2_cpFrame)
	} };

	::std::unique_ptr<D2D::BundleGlyph<GainerBundle>> systemGainer_pGlyph{
		new D2D::BundleGlyph<GainerBundle>{
			*pCanvas,
			{ 165, 40 },
			GainerBundle{ pActiveBrush }
	} };

	::std::shared_ptr<OutStateChangePolicy> pOutStateChangePolicy{
		new OutStateChangePolicy{}
	};
	::std::shared_ptr<GainerStateChangePolicy> pGainerStateChangePolicy{
		new GainerStateChangePolicy{}
	};
	auto out_a_1_onPromote = [this](const int& state)->void {
		if (remote.VBVMR_SetParameterFloat((char*)"Strip[5].A1", static_cast<float>(state))) {
			throw windows_error{ "Cannot enable A1" };
		}
	};
	using out_a_1_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_a_1_onPromote)>;
	::std::unique_ptr<out_a_1_StatePromotionPolicy> out_a_1_pStatePromotionPolicy{
		new out_a_1_StatePromotionPolicy{ out_a_1_onPromote }
	};
	auto out_a_2_onPromote = [this](const int& state)->void {
		if (remote.VBVMR_SetParameterFloat((char*)"Strip[5].A2", static_cast<float>(state))) {
			throw windows_error{ "Cannot enable A2" };
		}
	};
	using out_a_2_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_a_2_onPromote)>;
	::std::unique_ptr<out_a_2_StatePromotionPolicy> out_a_2_pStatePromotionPolicy{
		new out_a_2_StatePromotionPolicy{ out_a_2_onPromote }
	};
	auto out_b_1_onPromote = [this](const int& state)->void {
		if (remote.VBVMR_SetParameterFloat((char*)"Strip[5].B1", static_cast<float>(state))) {
			throw windows_error{ "Cannot enable B1" };
		}
	};
	using out_b_1_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_b_1_onPromote)>;
	::std::unique_ptr<out_b_1_StatePromotionPolicy> out_b_1_pStatePromotionPolicy{
		new out_b_1_StatePromotionPolicy{ out_b_1_onPromote }
	};
	auto out_b_2_onPromote = [this](const int& state)->void {
		if (remote.VBVMR_SetParameterFloat((char*)"Strip[5].B2", static_cast<float>(state))) {
			throw windows_error{ "Cannot enable B2" };
		}
	};
	using out_b_2_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(out_b_2_onPromote)>;
	::std::unique_ptr<out_b_2_StatePromotionPolicy> out_b_2_pStatePromotionPolicy{
		new out_b_2_StatePromotionPolicy{ out_b_2_onPromote }
	};
	auto systemGainer_onPromote = [this](const int& state)->void {
		if (remote.VBVMR_SetParameterFloat((char*)"Strip[5].Gain", static_cast<float>(state) / 10.F)) {
			throw windows_error{ "Cannot set system gainer level" };
		}
	};
	using systemGainer_StatePromotionPolicy = PreconfiguredStatePromotionPolicy<int, decltype(systemGainer_onPromote)>;
	::std::unique_ptr<systemGainer_StatePromotionPolicy> systemGainer_pStatePromotionPolicy{
		new systemGainer_StatePromotionPolicy{ systemGainer_onPromote }
	};
	auto onCarouseleGlyphUpdate = [](D2D::FrameGlyph& glyph, const int& state)->void {
		glyph.set_Frame(state);
	};
	using CarouseleGlyphUpdatePolicy = PreconfiguredGlyphUpdatePolicy<D2D::FrameGlyph, int, decltype(onCarouseleGlyphUpdate)>;
	::std::shared_ptr<CarouseleGlyphUpdatePolicy> pCarouseleGlyphUpdatePolicy{
		new CarouseleGlyphUpdatePolicy{ onCarouseleGlyphUpdate }
	};
	auto onGainerGlyphUpdate = [](D2D::BundleGlyph<GainerBundle>& glyph, const int& state)->void {
		glyph.get_Bundle()
			.set_Level((state + 600) / 10.F);
	};
	using GainerGlyphUpdatePolicy = PreconfiguredGlyphUpdatePolicy<D2D::BundleGlyph<GainerBundle>, int, decltype(onGainerGlyphUpdate)>;
	::std::shared_ptr<GainerGlyphUpdatePolicy> pGainerGlyphUpdatePolicy{
		new GainerGlyphUpdatePolicy{ onGainerGlyphUpdate }
	};
	using CarouselInteractivityPolicy = PreconfiguredInteractivityPolicy<Carousel,
		CarouselOnMouseLDown,
		OnMouseIgnore,
		OnMouseWheelIgnore,
		OnMouseIgnore,
		OnMouseIgnore>;
	::std::shared_ptr<CarouselInteractivityPolicy> pCarouselInteractivityPolicy{
		new CarouselInteractivityPolicy{
			CarouselOnMouseLDown{},
			OnMouseIgnore{},
			OnMouseWheelIgnore{},
			OnMouseIgnore{},
			OnMouseIgnore{}
	} };
	using GainerInteractivityPolicy = PreconfiguredInteractivityPolicy<Gainer,
		GainerOnMouseLDown,
		OnMouseIgnore,
		GainerOnMouseWheel,
		GainerOnMouseMove,
		GainerOnMouseLUp>;
	::std::shared_ptr<GainerInteractivityPolicy> pGainerInteractivityPolicy{
		new GainerInteractivityPolicy{
			GainerOnMouseLDown{ *pScene },
			OnMouseIgnore{},
			GainerOnMouseWheel{},
			GainerOnMouseMove{},
			GainerOnMouseLUp{ *pScene }
	} };

	::std::unique_ptr<IComponent> out_a_cpControl[]{
		::std::unique_ptr<Carousel>{
			new Carousel{
				{ 0, 0 }, { 0, 0 },
				::std::move(out_a_1_pGlyph),
				pOutStateChangePolicy,
				::std::move(out_a_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		} }, ::std::unique_ptr<Carousel>{
			new Carousel{
				{ 0, 2 }, { 0, 0 },
				::std::move(out_a_2_pGlyph),
				pOutStateChangePolicy,
				::std::move(out_a_2_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		} }
	};
	::std::unique_ptr<IComponent> out_b_cpControl[]{
		::std::unique_ptr<Carousel>{
			new Carousel{
				{ 0, 0 }, { 0, 0 },
				::std::move(out_b_1_pGlyph),
				pOutStateChangePolicy,
				::std::move(out_b_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		} }, ::std::unique_ptr<Carousel>{
			new Carousel{
				{ 0, 2 }, { 0, 0 },
				::std::move(out_b_2_pGlyph),
				pOutStateChangePolicy,
				::std::move(out_b_2_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		} }
	};

	::std::unique_ptr<IComponent> cpComponent[]{
		::std::unique_ptr<Gainer>{
			new Gainer{
				{ 0, 0 }, { 0, 0 },
				::std::move(systemGainer_pGlyph),
				pGainerStateChangePolicy,
				::std::move(systemGainer_pStatePromotionPolicy),
				pGainerGlyphUpdatePolicy,
				pGainerInteractivityPolicy
		} }, ::std::unique_ptr<StackPanel<Direction::Down>>{
			new StackPanel<Direction::Down>{
				{ 2, 0 },
				{ 0, 0 },
				::std::begin(out_a_cpControl),
				::std::end(out_a_cpControl)
		} }, ::std::unique_ptr<StackPanel<Direction::Down>>{
			new StackPanel<Direction::Down>{
				{ 2, 0 },
				{ 0, 0 },
				::std::begin(out_b_cpControl),
				::std::end(out_b_cpControl)
		} }
	};

	::std::unique_ptr<IComponent> pComposition{
		new StackPanel<Direction::Right>{
			{ 0, 0 },
			{ 0, 0 },
			::std::begin(cpComponent),
			::std::end(cpComponent)
	} };

	pScene->set_Canvas(::std::move(pCanvas));
	pScene->set_Composition(::std::move(pComposition));

	m_pScene.reset(pScene.get());
	pScene.release();
}