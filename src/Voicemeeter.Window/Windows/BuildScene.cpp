#include <memory>

#include "estd/type_traits.h"

#include "Windows/Wrappers.h"

#include "Voicemeeter.UI/Controls/Carousel.h"
#include "Voicemeeter.UI/Decorators/Margin.h"
#include "Voicemeeter.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.UI/Graphics/Glyphs/Frame.h"
#include "Voicemeeter.UI/Panels/Stack.h"
#include "Voicemeeter.UI/Policies/CarouselGlyphUpdate.h"
#include "Voicemeeter.UI/Policies/CarouselInteractivity.h"
#include "Voicemeeter.UI/Policies/IStatePromotion.h"
#include "Voicemeeter.UI/Policies/SliderInteractivity.h"
#include "Voicemeeter.UI/InputTracker.h"
#include "Voicemeeter.UI.D2D/Controls/Gainer.h"
#include "Voicemeeter.UI.D2D/Decorators/WindowsGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Gainer.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Mute.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Out.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Vban.h"
#include "Voicemeeter.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.UI.D2D/Graphics/Theme.h"
#include "Voicemeeter.UI.D2D/Policies/GainerGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/GainerInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/GainerStateChange.h"
#include "Voicemeeter.UI.D2D/Policies/CheckboxStateChange.h"
#include "Voicemeeter.UI.D2D/Scene.h"

#include "Window.h"

using namespace ::Voicemeeter::UI;

class EmptyStatePromotion : public Policies::IStatePromotion<int> {
public:
	EmptyStatePromotion() = default;
	EmptyStatePromotion(const EmptyStatePromotion&) = delete;
	EmptyStatePromotion(EmptyStatePromotion&&) = delete;

	~EmptyStatePromotion() = default;

	EmptyStatePromotion& operator=(const EmptyStatePromotion&) = delete;
	EmptyStatePromotion& operator=(EmptyStatePromotion&&) = delete;

	virtual void Promote(const int& state) const {

	};
};

using namespace ::Voicemeeter::Windows;

void Window::BuildScene() {
	::std::unique_ptr<InputTracker> pInputTracker{
		new InputTracker{ *this }
	};

	const D2D::Graphics::Theme theme{ D2D::Graphics::Theme::Default() };
	::std::unique_ptr<D2D::Graphics::Canvas> pCanvas{
		new D2D::Graphics::Canvas{ m_hWnd, theme }
	};

	::std::unique_ptr<IGlyph> out_a_1_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"A1"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"A1")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_a_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_a_1_cpFrame),
			::std::end(out_a_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_a_2_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"A2"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"A2")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_a_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_a_2_cpFrame),
			::std::end(out_a_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_b_1_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"B1"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"B1")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_b_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_b_1_cpFrame),
			::std::end(out_b_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_b_2_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"B2"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"B2")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_b_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_b_2_cpFrame),
			::std::end(out_b_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> systemMute_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Mute<false>>(
			*pCanvas),
		::std::make_unique<D2D::Graphics::Glyphs::Mute<true>>(
			*pCanvas)
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> systemMute_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(systemMute_cpFrame),
			::std::end(systemMute_cpFrame)
	} };

	::std::unique_ptr<IGlyph> vban_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Vban<false>>(
			*pCanvas),
		::std::make_unique<D2D::Graphics::Glyphs::Vban<true>>(
			*pCanvas)
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> vban_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(vban_cpFrame),
			::std::end(vban_cpFrame)
	} };

	::std::unique_ptr<D2D::Graphics::Glyphs::Gainer> systemGainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Gainer>(*pCanvas)
	};

	::std::shared_ptr<D2D::Policies::CheckboxStateChange> pCheckboxStateChangePolicy{
		new D2D::Policies::CheckboxStateChange{}
	};
	::std::shared_ptr<D2D::Policies::GainerStateChange> pGainerStateChangePolicy{
		new D2D::Policies::GainerStateChange{}
	};
	::std::unique_ptr<EmptyStatePromotion> out_a_1_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::unique_ptr<EmptyStatePromotion> out_a_2_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::unique_ptr<EmptyStatePromotion> out_b_1_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::unique_ptr<EmptyStatePromotion> out_b_2_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::unique_ptr<EmptyStatePromotion> systemMute_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::unique_ptr<EmptyStatePromotion> vban_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::unique_ptr<EmptyStatePromotion> systemGainer_pStatePromotionPolicy{
		new EmptyStatePromotion{}
	};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<Glyphs::Frame, int, Policies::CarouselGlyphUpdate>> pCarouseleGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<Glyphs::Frame, int, Policies::CarouselGlyphUpdate>{ m_hWnd }
	};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Gainer, int, D2D::Policies::GainerGlyphUpdate>> pGainerGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Gainer, int, D2D::Policies::GainerGlyphUpdate>{ m_hWnd }
	};
	::std::shared_ptr<CarouselInteractivity> pCarouselInteractivityPolicy{
		new Policies::CarouselInteractivity{}
	};
	::std::shared_ptr<D2D::Policies::GainerInteractivity> pGainerInteractivityPolicy{
		new D2D::Policies::GainerInteractivity{ *pInputTracker }
	};

	::std::unique_ptr<IComponent> out_a_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(out_a_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(out_a_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(out_a_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(out_a_2_pStatePromotionPolicy),
					pCarouseleGlyphUpdatePolicy,
					pCarouselInteractivityPolicy
		)
	};
	::std::unique_ptr<IComponent> out_b_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(out_b_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(out_b_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(out_b_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(out_b_2_pStatePromotionPolicy),
					pCarouseleGlyphUpdatePolicy,
					pCarouselInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> cpComponent[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(vban_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(vban_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Gainer>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(systemGainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(systemGainer_pStatePromotionPolicy),
					pGainerGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::begin(out_a_cpControl),
					::std::end(out_a_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::begin(out_b_cpControl),
					::std::end(out_b_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(systemMute_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(systemMute_pStatePromotionPolicy),
					pCarouseleGlyphUpdatePolicy,
					pCarouselInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> pComposition{
		new Panels::Stack<Panels::Direction::Right>{
			::std::begin(cpComponent),
			::std::end(cpComponent)
	} };

	m_pScene.reset(new D2D::Scene{
		::std::move(pInputTracker),
		::std::move(pCanvas),
		::std::move(pComposition)
	});
}