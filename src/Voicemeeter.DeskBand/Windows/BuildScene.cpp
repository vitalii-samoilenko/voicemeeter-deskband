#include <memory>

#include "estd/type_traits.h"

#include "Voicemeeter.UI/Controls/Carousel.h"
#include "Voicemeeter.UI/Decorators/Margin.h"
#include "Voicemeeter.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.UI/Graphics/Glyphs/Frame.h"
#include "Voicemeeter.UI/Panels/Stack.h"
#include "Voicemeeter.UI/Policies/CarouselGlyphUpdate.h"
#include "Voicemeeter.UI/Policies/CarouselInteractivity.h"
#include "Voicemeeter.UI/Policies/CheckboxStateChange.h"
#include "Voicemeeter.UI/Policies/IStatePromotion.h"
#include "Voicemeeter.UI/Policies/SliderInteractivity.h"
#include "Voicemeeter.UI/States/Knob.h"
#include "Voicemeeter.UI/InputTracker.h"
#include "Voicemeeter.UI.D2D/Controls/Gainer.h"
#include "Voicemeeter.UI.D2D/Controls/Knob.h"
#include "Voicemeeter.UI.D2D/Decorators/WindowsGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Gainer.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Knob.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Mute.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Out.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Vban.h"
#include "Voicemeeter.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.UI.D2D/Graphics/Theme.h"
#include "Voicemeeter.UI.D2D/Policies/GainerGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/GainerInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/GainerStateChange.h"
#include "Voicemeeter.UI.D2D/Policies/KnobGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/KnobInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/KnobStateChange.h"
#include "Voicemeeter.UI.D2D/Scene.h"

#include "DeskBand.h"

using namespace ::Voicemeeter::UI;

using namespace ::Voicemeeter::Windows;

template<typename TMapper>
class RemoteCheckboxStatePromotion : public Policies::IStatePromotion<int> {
	static_assert(
		::estd::is_invocable_r<float, TMapper, const int&>(),
		"TMapper must be invocable with const int& and must return float");

public:
	RemoteCheckboxStatePromotion(
		const T_VBVMR_INTERFACE& remote,
		const char* pLabel,
		TMapper mapper
	) : m_remote{ remote }
	  , m_pLabel{ pLabel }
	  , m_mapper{ mapper } {

	};
	RemoteCheckboxStatePromotion() = delete;
	RemoteCheckboxStatePromotion(const RemoteCheckboxStatePromotion&) = delete;
	RemoteCheckboxStatePromotion(RemoteCheckboxStatePromotion&&) = delete;

	~RemoteCheckboxStatePromotion() = default;

	RemoteCheckboxStatePromotion& operator=(const RemoteCheckboxStatePromotion&) = delete;
	RemoteCheckboxStatePromotion& operator=(RemoteCheckboxStatePromotion&&) = delete;

	virtual void Promote(const int& state) const {
		if (m_remote.VBVMR_SetParameterFloat(const_cast<char*>(m_pLabel), m_mapper(state))) {
			throw ::Windows::Error{ m_pLabel };
		}
	};

private:
	const T_VBVMR_INTERFACE& m_remote;
	const char* m_pLabel;
	TMapper m_mapper;
};

template<typename TLevelMapper, typename TPinnedMapper>
class RemoteKnobStatePromotion : public Policies::IStatePromotion<States::Knob> {
	static_assert(
		::estd::is_invocable_r<float, TLevelMapper, const int&>(),
		"TLevelMapper must be invocable with const int& and must return float");
	static_assert(
		::estd::is_invocable_r<float, TPinnedMapper, const bool&>(),
		"TPinnedMapper must be invocable with const bool& and must return float");

public:
	RemoteKnobStatePromotion(
		const T_VBVMR_INTERFACE& remote,
		const char* pLevelLabel,
		const char* pPinnedLabel,
		TLevelMapper levelMapper,
		TPinnedMapper pinnedMapper
	) : m_remote{ remote }
	  , m_pLevelLabel{ pLevelLabel }
	  , m_pPinnedLabel{ pPinnedLabel }
	  , m_levelMapper{ levelMapper } 
	  , m_pinnedMapper{ pinnedMapper } {

	};
	RemoteKnobStatePromotion() = delete;
	RemoteKnobStatePromotion(const RemoteKnobStatePromotion&) = delete;
	RemoteKnobStatePromotion(RemoteKnobStatePromotion&&) = delete;

	~RemoteKnobStatePromotion() = default;

	RemoteKnobStatePromotion& operator=(const RemoteKnobStatePromotion&) = delete;
	RemoteKnobStatePromotion& operator=(RemoteKnobStatePromotion&&) = delete;

	virtual void Promote(const States::Knob& state) const {
		if (m_remote.VBVMR_SetParameterFloat(const_cast<char*>(m_pLevelLabel), m_levelMapper(state.level))) {
			throw ::Windows::Error{ m_pLevelLabel };
		}
		if (m_remote.VBVMR_SetParameterFloat(const_cast<char*>(m_pPinnedLabel), m_pinnedMapper(state.pinned))) {
			throw ::Windows::Error{ m_pPinnedLabel };
		}
	};

private:
	const T_VBVMR_INTERFACE& m_remote;
	const char* m_pLevelLabel;
	const char* m_pPinnedLabel;
	TLevelMapper m_levelMapper;
	TPinnedMapper m_pinnedMapper;
};

void DeskBand::BuildScene() {
	auto checkboxMap = [](const int& state)->float {
		return static_cast<float>(state);
	};
	auto gainerMap = [](const int& state)->float {
		return (state / 100.F - 90.F) / 3.75;
	};
	using CheckboxStatePromotion = RemoteCheckboxStatePromotion<decltype(checkboxMap)>;
	using GainerStatePromotion = RemoteKnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>;
	::std::unique_ptr<CheckboxStatePromotion> system_out_a_1_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].A1", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> system_out_a_2_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].A2", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> system_out_b_1_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].B1", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> system_out_b_2_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[5].B2", checkboxMap }
	};
	//::std::unique_ptr<CheckboxStatePromotion> system_systemMute_pStatePromotionPolicy{
	//	new CheckboxStatePromotion{ m_remote, "Strip[5].Mute", checkboxMap }
	//};
	//::std::unique_ptr<GainerStatePromotion> systemGainer_pStatePromotionPolicy{
	//	new GainerStatePromotion{ m_remote, "Strip[5].Gain", gainerMap }
	//};
	::std::unique_ptr<GainerStatePromotion> systemGainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Strip[5].Gain", "Strip[5].Mute", gainerMap, checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> micro_out_a_1_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[0].A1", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> micro_out_a_2_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[0].A2", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> micro_out_b_1_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[0].B1", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> micro_out_b_2_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[0].B2", checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> micro_systemMute_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "Strip[0].Mute", checkboxMap }
	};
	::std::unique_ptr<GainerStatePromotion> microGainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Strip[0].Gain", "Strip[0].Mute", gainerMap, checkboxMap }
	};
	::std::unique_ptr<GainerStatePromotion> a1Gainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Bus[0].Gain", "Bus[0].Mute", gainerMap, checkboxMap }
	};
	::std::unique_ptr<GainerStatePromotion> a2Gainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Bus[1].Gain", "Bus[1].Mute", gainerMap, checkboxMap }
	};
	::std::unique_ptr<GainerStatePromotion> b1Gainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Bus[5].Gain", "Bus[5].Mute", gainerMap, checkboxMap }
	};
	::std::unique_ptr<GainerStatePromotion> b2Gainer_pStatePromotionPolicy{
		new GainerStatePromotion{ m_remote, "Bus[6].Gain", "Bus[6].Mute", gainerMap, checkboxMap }
	};
	::std::unique_ptr<CheckboxStatePromotion> vban_pStatePromotionPolicy{
		new CheckboxStatePromotion{ m_remote, "vban.Enable", checkboxMap }
	};

	::std::unique_ptr<InputTracker> pInputTracker{
	new InputTracker{ *this }
	};

	const D2D::Graphics::Theme theme{ D2D::Graphics::Theme::Default() };
	::std::unique_ptr<D2D::Graphics::Canvas> pCanvas{
		new D2D::Graphics::Canvas{ m_hWnd, theme }
	};

	::std::unique_ptr<IGlyph> system_out_a_1_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"A1"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"A1")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> system_out_a_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(system_out_a_1_cpFrame),
			::std::end(system_out_a_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> system_out_a_2_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"A2"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"A2")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> system_out_a_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(system_out_a_2_cpFrame),
			::std::end(system_out_a_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> system_out_b_1_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"B1"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"B1")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> system_out_b_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(system_out_b_1_cpFrame),
			::std::end(system_out_b_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> system_out_b_2_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"B2"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"B2")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> system_out_b_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(system_out_b_2_cpFrame),
			::std::end(system_out_b_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> micro_out_a_1_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"A1"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"A1")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> micro_out_a_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(micro_out_a_1_cpFrame),
			::std::end(micro_out_a_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> micro_out_a_2_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"A2"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"A2")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> micro_out_a_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(micro_out_a_2_cpFrame),
			::std::end(micro_out_a_2_cpFrame)
	} };

	::std::unique_ptr<IGlyph> micro_out_b_1_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"B1"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"B1")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> micro_out_b_1_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(micro_out_b_1_cpFrame),
			::std::end(micro_out_b_1_cpFrame)
	} };

	::std::unique_ptr<IGlyph> micro_out_b_2_cpFrame[]{
		::std::make_unique<D2D::Graphics::Glyphs::Out<false>>(
			*pCanvas, L"B2"),
		::std::make_unique<D2D::Graphics::Glyphs::Out<true>>(
			*pCanvas, L"B2")
	};
	::std::unique_ptr<Graphics::Glyphs::Frame> micro_out_b_2_pGlyph{
		new Graphics::Glyphs::Frame{
			::std::begin(micro_out_b_2_cpFrame),
			::std::end(micro_out_b_2_cpFrame)
	} };

	//::std::unique_ptr<IGlyph> systemMute_cpFrame[]{
	//	::std::make_unique<D2D::Graphics::Glyphs::Mute<false>>(
	//		*pCanvas),
	//	::std::make_unique<D2D::Graphics::Glyphs::Mute<true>>(
	//		*pCanvas)
	//};
	//::std::unique_ptr<Graphics::Glyphs::Frame> systemMute_pGlyph{
	//	new Graphics::Glyphs::Frame{
	//		::std::begin(systemMute_cpFrame),
	//		::std::end(systemMute_cpFrame)
	//} };

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

	//::std::unique_ptr<D2D::Graphics::Glyphs::Gainer> systemGainer_pGlyph{
	//	::std::make_unique<D2D::Graphics::Glyphs::Gainer>(*pCanvas)
	//};
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> systemGainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Knob>(
			*pCanvas, L"C2")
	};
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> microGainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Knob>(
			*pCanvas, L"C1")
	};
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> a1Gainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Knob>(
			*pCanvas, L"A1")
	};
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> a2Gainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Knob>(
			*pCanvas, L"A2")
	};
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> b1Gainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Knob>(
			*pCanvas, L"B1")
	};
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> b2Gainer_pGlyph{
		::std::make_unique<D2D::Graphics::Glyphs::Knob>(
			*pCanvas, L"B2")
	};

	::std::shared_ptr<Policies::CheckboxStateChange> pCheckboxStateChangePolicy{
		new Policies::CheckboxStateChange{}
	};
	//::std::shared_ptr<D2D::Policies::GainerStateChange> pGainerStateChangePolicy{
	//	new D2D::Policies::GainerStateChange{}
	//};
	::std::shared_ptr<D2D::Policies::KnobStateChange> pGainerStateChangePolicy{
		new D2D::Policies::KnobStateChange{}
	};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<Glyphs::Frame, int, Policies::CarouselGlyphUpdate>> pCarouseleGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<Glyphs::Frame, int, Policies::CarouselGlyphUpdate>{ m_hWnd }
	};
	//::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Gainer, int, D2D::Policies::GainerGlyphUpdate>> pGainerGlyphUpdatePolicy{
	//	new D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Gainer, int, D2D::Policies::GainerGlyphUpdate>{ m_hWnd }
	//};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Knob, States::Knob, D2D::Policies::KnobGlyphUpdate>> pGainerGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Knob, States::Knob, D2D::Policies::KnobGlyphUpdate>{ m_hWnd }
	};
	::std::shared_ptr<CarouselInteractivity> pCarouselInteractivityPolicy{
		new Policies::CarouselInteractivity{}
	};
	//::std::shared_ptr<D2D::Policies::GainerInteractivity> pGainerInteractivityPolicy{
	//	new D2D::Policies::GainerInteractivity{ *pInputTracker }
	//};
	::std::shared_ptr<D2D::Policies::KnobInteractivity> pGainerInteractivityPolicy{
		new D2D::Policies::KnobInteractivity{ *pInputTracker }
	};

	::std::unique_ptr<IComponent> system_out_a_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(system_out_a_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(system_out_a_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(system_out_a_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(system_out_a_2_pStatePromotionPolicy),
					pCarouseleGlyphUpdatePolicy,
					pCarouselInteractivityPolicy
		)
	};
	::std::unique_ptr<IComponent> system_out_b_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(system_out_b_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(system_out_b_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(system_out_b_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(system_out_b_2_pStatePromotionPolicy),
					pCarouseleGlyphUpdatePolicy,
					pCarouselInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> micro_out_a_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(micro_out_a_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(micro_out_a_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(micro_out_a_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(micro_out_a_2_pStatePromotionPolicy),
					pCarouseleGlyphUpdatePolicy,
					pCarouselInteractivityPolicy
		)
	};
	::std::unique_ptr<IComponent> micro_out_b_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			Controls::Carousel>>(
				*pInputTracker,
				::std::move(micro_out_b_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(micro_out_b_1_pStatePromotionPolicy),
				pCarouseleGlyphUpdatePolicy,
				pCarouselInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Controls::Carousel>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(micro_out_b_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(micro_out_b_2_pStatePromotionPolicy),
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
				D2D::Controls::Knob>>>(
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
					::std::begin(system_out_a_cpControl),
					::std::end(system_out_a_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::begin(system_out_b_cpControl),
					::std::end(system_out_b_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(microGainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(microGainer_pStatePromotionPolicy),
					pGainerGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::begin(micro_out_a_cpControl),
					::std::end(micro_out_a_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::begin(micro_out_b_cpControl),
					::std::end(micro_out_b_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(a1Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(a1Gainer_pStatePromotionPolicy),
					pGainerGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(a2Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(a2Gainer_pStatePromotionPolicy),
					pGainerGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(b1Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(b1Gainer_pStatePromotionPolicy),
					pGainerGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(b2Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(b2Gainer_pStatePromotionPolicy),
					pGainerGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		)/*, ::std::make_unique<Decorators::Margin<
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
		)*/
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