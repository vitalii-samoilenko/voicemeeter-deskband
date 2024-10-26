#include <memory>

#include "estd/type_traits.h"

#include "Voicemeeter.DeskBand.UI/Controls/Carousel.h"
#include "Voicemeeter.DeskBand.UI/Decorators/Margin.h"
#include "Voicemeeter.DeskBand.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.DeskBand.UI/Graphics/Glyphs/Frame.h"
#include "Voicemeeter.DeskBand.UI/Panels/Stack.h"
#include "Voicemeeter.DeskBand.UI/Policies/CarouselGlyphUpdate.h"
#include "Voicemeeter.DeskBand.UI/Policies/CarouselInteractivity.h"
#include "Voicemeeter.DeskBand.UI/Policies/IStatePromotion.h"
#include "Voicemeeter.DeskBand.UI/Policies/SliderInteractivity.h"
#include "Voicemeeter.DeskBand.UI/InputTracker.h"
#include "Voicemeeter.DeskBand.UI/Scene.h"
#include "Voicemeeter.DeskBand.UI.D2D/Controls/Gainer.h"
#include "Voicemeeter.DeskBand.UI.D2D/Decorators/InstantRendering.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Glyphs/Gainer.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Glyphs/Out.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/GainerGlyphUpdate.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/GainerInteractivity.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/GainerStateChange.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/OutStateChange.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Window.h"

using namespace ::Voicemeeter::DeskBand::Windows;

using namespace ::Voicemeeter::DeskBand::UI;

template<typename TMapper>
class RemoteStatePromotion : public Policies::IStatePromotion<int> {
	static_assert(
		::estd::is_invocable_r<float, TMapper, const int&>(),
		"TMapper must be invocable with const int& and must return float");

public:
	RemoteStatePromotion(
		const T_VBVMR_INTERFACE& remote,
		const char* pLabel,
		TMapper mapper
	) : m_remote{ remote }
	  , m_pLabel{ pLabel }
	  , m_mapper{ mapper } {

	};
	RemoteStatePromotion() = delete;
	RemoteStatePromotion(const RemoteStatePromotion&) = delete;
	RemoteStatePromotion(RemoteStatePromotion&&) = delete;

	~RemoteStatePromotion() = default;

	RemoteStatePromotion& operator=(const RemoteStatePromotion&) = delete;
	RemoteStatePromotion& operator=(RemoteStatePromotion&&) = delete;

	virtual void Promote(const int& state) const {
		//if (m_remote.VBVMR_SetParameterFloat(const_cast<char*>(m_pLabel), m_mapper(state))) {
			//throw windows_error{ m_pLabel };
		//}
	};

private:
	const T_VBVMR_INTERFACE& m_remote;
	const char* m_pLabel;
	TMapper m_mapper;
};

void Window::BuildScene() {
	::std::unique_ptr<InputTracker> pInputTracker{
		new InputTracker{ *this }
	};

	::std::unique_ptr<D2D::Graphics::Canvas> pCanvas{
		new D2D::Graphics::Canvas{ m_hWnd }
	};

	::std::unique_ptr<IGlyph> out_a_1_cpFrame[]{
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<false>>>(*pCanvas),
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<true>>>(*pCanvas)
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_a_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_a_1_cpFrame),
			::std::end(out_a_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_a_2_cpFrame[]{
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<false>>>(*pCanvas),
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<true>>>(*pCanvas)
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_a_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_a_2_cpFrame),
			::std::end(out_a_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_b_1_cpFrame[]{
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<false>>>(*pCanvas),
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<true>>>(*pCanvas)
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_b_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_b_1_cpFrame),
			::std::end(out_b_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> out_b_2_cpFrame[]{
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<false>>>(*pCanvas),
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Out<true>>>(*pCanvas)
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> out_b_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(out_b_2_cpFrame),
			::std::end(out_b_2_cpFrame)
	} };

	::std::unique_ptr<D2D::Graphics::Glyphs::Gainer> systemGainer_pGlyph{
		::std::make_unique<D2D::Decorators::InstantRendering<
			D2D::Graphics::Glyphs::Gainer>>(*pCanvas)
	};

	::std::shared_ptr<D2D::Policies::OutStateChange> pOutStateChangePolicy{
		new D2D::Policies::OutStateChange{}
	};
	::std::shared_ptr<D2D::Policies::GainerStateChange> pGainerStateChangePolicy{
		new D2D::Policies::GainerStateChange{}
	};
	auto outMap = [](const int& state)->float {
		return static_cast<float>(state);
	};
	auto gainerMap = [](const int& state)->float {
		return state / 100.F - 28.F - 60.F;
	};
	using OutStatePromotion = RemoteStatePromotion<decltype(outMap)>;
	using GainerStatePromotion = RemoteStatePromotion<decltype(gainerMap)>;
	::std::unique_ptr<OutStatePromotion> out_a_1_pStatePromotionPolicy{
		new OutStatePromotion{ m_remote, "Strip[5].A1", outMap }
	};
	::std::unique_ptr<OutStatePromotion> out_a_2_pStatePromotionPolicy{
		new OutStatePromotion{ m_remote, "Strip[5].A2", outMap }
	};
	::std::unique_ptr<OutStatePromotion> out_b_1_pStatePromotionPolicy{
		new OutStatePromotion{ m_remote, "Strip[5].B1", outMap }
	};
	::std::unique_ptr<OutStatePromotion> out_b_2_pStatePromotionPolicy{
		new OutStatePromotion{ m_remote, "Strip[5].B2", outMap }
	};
	::std::unique_ptr<GainerStatePromotion> systemGainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Strip[5].Gain", gainerMap }
	};
	::std::shared_ptr<Policies::CarouselGlyphUpdate> pCarouseleGlyphUpdatePolicy{
		new Policies::CarouselGlyphUpdate{}
	};
	::std::shared_ptr<D2D::Policies::GainerGlyphUpdate> pGainerGlyphUpdatePolicy{
		new D2D::Policies::GainerGlyphUpdate{}
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
				::std::move(out_a_1_pGlyph),
				pOutStateChangePolicy,
				::std::move(out_a_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::RegionCheck<
			Decorators::Margin<
				Decorators::RegionCheck<
					Controls::Carousel>>>>(
						::linear_algebra::vectord{ 0, 2 },
						::linear_algebra::vectord{ 0, 0 },
						::std::move(out_a_2_pGlyph),
						pOutStateChangePolicy,
						::std::move(out_a_2_pStatePromotionPolicy),
						pCarouseleGlyphUpdatePolicy,
						pCarouselInteractivityPolicy
		)
	};
	::std::unique_ptr<IComponent> out_b_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				::std::move(out_b_1_pGlyph),
				pOutStateChangePolicy,
				::std::move(out_b_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::RegionCheck<
			Decorators::Margin<
				Decorators::RegionCheck<
					Controls::Carousel>>>>(
						::linear_algebra::vectord{ 0, 2 },
						::linear_algebra::vectord{ 0, 0 },
						::std::move(out_b_2_pGlyph),
						pOutStateChangePolicy,
						::std::move(out_b_2_pStatePromotionPolicy),
						pCarouseleGlyphUpdatePolicy,
						pCarouselInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> cpComponent[]{
		::std::make_unique<Decorators::RegionCheck<
			D2D::Controls::Gainer>>(
				::std::move(systemGainer_pGlyph),
				pGainerStateChangePolicy,
				::std::move(systemGainer_pStatePromotionPolicy),
				pGainerGlyphUpdatePolicy,
				pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::RegionCheck<
			Decorators::Margin<
				Decorators::RegionCheck<
					Panels::Stack<Panels::Direction::Down>>>>>(
						::linear_algebra::vectord{ 2, 0 },
						::linear_algebra::vectord{ 0, 0 },
						::std::begin(out_a_cpControl),
						::std::end(out_a_cpControl)
		), ::std::make_unique<Decorators::RegionCheck<
			Decorators::Margin<
				Decorators::RegionCheck<
					Panels::Stack<Panels::Direction::Down>>>>>(
						::linear_algebra::vectord{ 2, 0 },
						::linear_algebra::vectord{ 0, 0 },
						::std::begin(out_b_cpControl),
						::std::end(out_b_cpControl)
		)
	};

	::std::unique_ptr<IComponent> pComposition{
		new Panels::Stack<Panels::Direction::Right>{
			::std::begin(cpComponent),
			::std::end(cpComponent)
	} };

	m_pScene.reset(new Scene{
		::std::move(pInputTracker),
		::std::move(pCanvas),
		::std::move(pComposition)
	});
}