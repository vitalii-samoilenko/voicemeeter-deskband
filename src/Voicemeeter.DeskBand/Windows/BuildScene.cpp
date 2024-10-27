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
#include "Voicemeeter.DeskBand.UI.D2D/Controls/Gainer.h"
#include "Voicemeeter.DeskBand.UI.D2D/Decorators/WindowsGlyphUpdate.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Glyphs/Gainer.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Glyphs/Mute.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Glyphs/Out.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Glyphs/Vban.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.DeskBand.UI.D2D/Graphics/Theme.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/GainerGlyphUpdate.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/GainerInteractivity.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/GainerStateChange.h"
#include "Voicemeeter.DeskBand.UI.D2D/Policies/CheckboxStateChange.h"
#include "Voicemeeter.DeskBand.UI.D2D/Scene.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "DeskBand.h"

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
		if (m_remote.VBVMR_SetParameterFloat(const_cast<char*>(m_pLabel), m_mapper(state))) {
			throw windows_error{ m_pLabel };
		}
	};

private:
	const T_VBVMR_INTERFACE& m_remote;
	const char* m_pLabel;
	TMapper m_mapper;
};

void DeskBand::BuildScene() {
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
	auto checkboxMap = [](const int& state)->float {
		return static_cast<float>(state);
		};
	auto gainerMap = [](const int& state)->float {
		return state / 100.F - 118.F;
		};
	using CheckboxStatePromotion = RemoteStatePromotion<decltype(checkboxMap)>;
	using GainerStatePromotion = RemoteStatePromotion<decltype(gainerMap)>;
	::std::unique_ptr<CheckboxStatePromotion> out_a_1_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].A1", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> out_a_2_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].A2", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> out_b_1_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].B1", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> out_b_2_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].B2", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> systemMute_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].Mute", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> vban_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "vban.Enable", checkboxMap }
	};
	::std::unique_ptr<GainerStatePromotion> systemGainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Strip[5].Gain", gainerMap }
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