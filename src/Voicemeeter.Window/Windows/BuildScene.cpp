#include <memory>

#include "estd/type_traits.h"

#include "Windows/Wrappers.h"

#include "Voicemeeter.UI/Decorators/Margin.h"
#include "Voicemeeter.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.UI/Panels/Stack.h"
#include "Voicemeeter.UI/Policies/CheckboxStateChange.h"
#include "Voicemeeter.UI/Policies/KnobStateChange.h"
#include "Voicemeeter.UI/Policies/IStatePromotion.h"
#include "Voicemeeter.UI/States/Knob.h"
#include "Voicemeeter.UI/FocusTracker.h"
#include "Voicemeeter.UI/InputTracker.h"
#include "Voicemeeter.UI.D2D/Controls/Knob.h"
#include "Voicemeeter.UI.D2D/Controls/Out.h"
#include "Voicemeeter.UI.D2D/Controls/Vban.h"
#include "Voicemeeter.UI.D2D/Decorators/WindowsGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Knob.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Out.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Vban.h"
#include "Voicemeeter.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.UI.D2D/Graphics/Theme.h"
#include "Voicemeeter.UI.D2D/Policies/KnobGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/KnobInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/KnobStateChange.h"
#include "Voicemeeter.UI.D2D/Policies/OutGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/OutInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/VbanGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/VbanInteractivity.h"
#include "Voicemeeter.UI.D2D/Scene.h"

#include "Window.h"

using namespace ::Voicemeeter::UI;

template<typename TState>
class EmptyStatePromotion : public Policies::IStatePromotion<TState> {
public:
	EmptyStatePromotion() = default;
	EmptyStatePromotion(const EmptyStatePromotion&) = delete;
	EmptyStatePromotion(EmptyStatePromotion&&) = delete;

	~EmptyStatePromotion() = default;

	EmptyStatePromotion& operator=(const EmptyStatePromotion&) = delete;
	EmptyStatePromotion& operator=(EmptyStatePromotion&&) = delete;

	virtual void Promote(const TState& state) const {

	};
};

using namespace ::Voicemeeter::Windows;

void Window::BuildScene() {
	::std::unique_ptr<FocusTracker> pFocusTracker{
		new FocusTracker{}
	};
	::std::unique_ptr<InputTracker> pInputTracker{
		new InputTracker{ *this }
	};

	const D2D::Graphics::Theme theme{ D2D::Graphics::Theme::Default() };
	::std::unique_ptr<D2D::Graphics::Canvas> pCanvas{
		new D2D::Graphics::Canvas{ m_hWnd, theme }
	};

	::std::unique_ptr<D2D::Graphics::Glyphs::Out> system_out_a_1_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"A1"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Out> system_out_a_2_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"A2"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Out> system_out_b_1_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"B1"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Out> system_out_b_2_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"B2"
	} };

	::std::unique_ptr<D2D::Graphics::Glyphs::Out> micro_out_a_1_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"A1"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Out> micro_out_a_2_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"A2"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Out> micro_out_b_1_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"B1"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Out> micro_out_b_2_pGlyph{
		new D2D::Graphics::Glyphs::Out{
			*pCanvas, L"B2"
	} };

	::std::unique_ptr<D2D::Graphics::Glyphs::Vban> vban_pGlyph{
		new D2D::Graphics::Glyphs::Vban{
			*pCanvas
	} };

	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> systemGainer_pGlyph{
		new D2D::Graphics::Glyphs::Knob{
			*pCanvas, L"V"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> microGainer_pGlyph{
		new D2D::Graphics::Glyphs::Knob{
			*pCanvas, L"P"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> a1Gainer_pGlyph{
		new D2D::Graphics::Glyphs::Knob{
			*pCanvas, L"A1"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> a2Gainer_pGlyph{
		new D2D::Graphics::Glyphs::Knob{
			*pCanvas, L"A2"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> b1Gainer_pGlyph{
		new D2D::Graphics::Glyphs::Knob{
			*pCanvas, L"B1"
	} };
	::std::unique_ptr<D2D::Graphics::Glyphs::Knob> b2Gainer_pGlyph{
		new D2D::Graphics::Glyphs::Knob{
			*pCanvas, L"B2"
	} };

	::std::shared_ptr<Policies::CheckboxStateChange> pCheckboxStateChangePolicy{
		new Policies::CheckboxStateChange{}
	};
	::std::shared_ptr<D2D::Policies::KnobStateChange> pGainerStateChangePolicy{
		new D2D::Policies::KnobStateChange{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> system_out_a_1_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> system_out_a_2_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> system_out_b_1_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> system_out_b_2_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> micro_out_a_1_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> micro_out_a_2_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> micro_out_b_1_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> micro_out_b_2_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<int>> vban_pStatePromotionPolicy{
		new EmptyStatePromotion<int>{}
	};
	::std::unique_ptr<EmptyStatePromotion<States::Knob>> systemGainer_pStatePromotionPolicy{
		new EmptyStatePromotion<States::Knob>{}
	};
	::std::unique_ptr<EmptyStatePromotion<States::Knob>> microGainer_pStatePromotionPolicy{
		new EmptyStatePromotion<States::Knob>{}
	};
	::std::unique_ptr<EmptyStatePromotion<States::Knob>> a1Gainer_pStatePromotionPolicy{
		new EmptyStatePromotion<States::Knob>{}
	};
	::std::unique_ptr<EmptyStatePromotion<States::Knob>> a2Gainer_pStatePromotionPolicy{
		new EmptyStatePromotion<States::Knob>{}
	};
	::std::unique_ptr<EmptyStatePromotion<States::Knob>> b1Gainer_pStatePromotionPolicy{
		new EmptyStatePromotion<States::Knob>{}
	};
	::std::unique_ptr<EmptyStatePromotion<States::Knob>> b2Gainer_pStatePromotionPolicy{
		new EmptyStatePromotion<States::Knob>{}
	};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Out, int, D2D::Policies::OutGlyphUpdate>> pOutGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Out, int, D2D::Policies::OutGlyphUpdate>{ m_hWnd }
	};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Vban, int, D2D::Policies::VbanGlyphUpdate>> pVbanGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Vban, int, D2D::Policies::VbanGlyphUpdate>{ m_hWnd }
	};
	::std::shared_ptr<D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Knob, States::Knob, D2D::Policies::KnobGlyphUpdate>> pKnobGlyphUpdatePolicy{
		new D2D::Decorators::WindowsGlyphUpdate<D2D::Graphics::Glyphs::Knob, States::Knob, D2D::Policies::KnobGlyphUpdate>{ m_hWnd }
	};
	::std::shared_ptr<D2D::Policies::OutInteractivity> pOutInteractivityPolicy{
		new D2D::Policies::OutInteractivity{ *pFocusTracker }
	};
	::std::shared_ptr<D2D::Policies::VbanInteractivity> pVbanInteractivityPolicy{
		new D2D::Policies::VbanInteractivity{ *pFocusTracker }
	};
	::std::shared_ptr<D2D::Policies::KnobInteractivity> pGainerInteractivityPolicy{
		new D2D::Policies::KnobInteractivity{ *pInputTracker, *pFocusTracker , *m_pTimer }
	};

	::std::unique_ptr<IComponent> system_out_a_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			D2D::Controls::Out>>(
				*pInputTracker,
				::std::move(system_out_a_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(system_out_a_1_pStatePromotionPolicy),
				pOutGlyphUpdatePolicy,
				pOutInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Out>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(system_out_a_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(system_out_a_2_pStatePromotionPolicy),
					pOutGlyphUpdatePolicy,
					pOutInteractivityPolicy
		)
	};
	::std::unique_ptr<IComponent> system_out_b_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			D2D::Controls::Out>>(
				*pInputTracker,
				::std::move(system_out_b_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(system_out_b_1_pStatePromotionPolicy),
				pOutGlyphUpdatePolicy,
				pOutInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Out>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(system_out_b_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(system_out_b_2_pStatePromotionPolicy),
					pOutGlyphUpdatePolicy,
					pOutInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> micro_out_a_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			D2D::Controls::Out>>(
				*pInputTracker,
				::std::move(micro_out_a_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(micro_out_a_1_pStatePromotionPolicy),
				pOutGlyphUpdatePolicy,
				pOutInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Out>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(micro_out_a_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(micro_out_a_2_pStatePromotionPolicy),
					pOutGlyphUpdatePolicy,
					pOutInteractivityPolicy
		)
	};
	::std::unique_ptr<IComponent> micro_out_b_cpControl[]{
		::std::make_unique<Decorators::RegionCheck<
			D2D::Controls::Out>>(
				*pInputTracker,
				::std::move(micro_out_b_1_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(micro_out_b_1_pStatePromotionPolicy),
				pOutGlyphUpdatePolicy,
				pOutInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Out>>>(
					::linear_algebra::vectord{ 0, 2 },
					::linear_algebra::vectord{ 0, 0 },
					*pInputTracker,
					::std::move(micro_out_b_2_pGlyph),
					pCheckboxStateChangePolicy,
					::std::move(micro_out_b_2_pStatePromotionPolicy),
					pOutGlyphUpdatePolicy,
					pOutInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> cpComponent[]{
		::std::make_unique<Decorators::RegionCheck<
			D2D::Controls::Vban>>(
				*pInputTracker,
				::std::move(vban_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(vban_pStatePromotionPolicy),
				pVbanGlyphUpdatePolicy,
				pVbanInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::move(microGainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(microGainer_pStatePromotionPolicy),
					pKnobGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::begin(micro_out_a_cpControl),
					::std::end(micro_out_a_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::begin(micro_out_b_cpControl),
					::std::end(micro_out_b_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::move(systemGainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(systemGainer_pStatePromotionPolicy),
					pKnobGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::begin(system_out_a_cpControl),
					::std::end(system_out_a_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				Panels::Stack<Panels::Direction::Down>>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::begin(system_out_b_cpControl),
					::std::end(system_out_b_cpControl)
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::move(a1Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(a1Gainer_pStatePromotionPolicy),
					pKnobGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::move(a2Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(a2Gainer_pStatePromotionPolicy),
					pKnobGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::move(b1Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(b1Gainer_pStatePromotionPolicy),
					pKnobGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		), ::std::make_unique<Decorators::Margin<
			Decorators::RegionCheck<
				D2D::Controls::Knob>>>(
					::linear_algebra::vectord{ 2, 0 },
					::linear_algebra::vectord{ 0, 0 },
					* pInputTracker,
					::std::move(b2Gainer_pGlyph),
					pGainerStateChangePolicy,
					::std::move(b2Gainer_pStatePromotionPolicy),
					pKnobGlyphUpdatePolicy,
					pGainerInteractivityPolicy
		)
	};

	::std::unique_ptr<IComponent> pComposition{
		new Panels::Stack<Panels::Direction::Right>{
			::std::begin(cpComponent),
			::std::end(cpComponent)
	} };

	m_pScene.reset(new D2D::Scene{
		::std::move(pInputTracker),
		::std::move(pFocusTracker),
		::std::move(pCanvas),
		::std::move(pComposition)
	});
}