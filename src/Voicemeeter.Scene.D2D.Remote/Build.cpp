#include <codecvt>
#include <locale>
#include <memory>
#include <vector>
#include <utility>

#include "estd/type_traits.h"

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
#include "Voicemeeter.UI.D2D/Controls/Plug.h"
#include "Voicemeeter.UI.D2D/Controls/Vban.h"
#include "Voicemeeter.UI.D2D/Decorators/WindowsGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Knob.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Plug.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Vban.h"
#include "Voicemeeter.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.UI.D2D/Graphics/Theme.h"
#include "Voicemeeter.UI.D2D/Policies/KnobGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/KnobInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/KnobStateChange.h"
#include "Voicemeeter.UI.D2D/Policies/PlugGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/PlugInteractivity.h"
#include "Voicemeeter.UI.D2D/Policies/VbanGlyphUpdate.h"
#include "Voicemeeter.UI.D2D/Policies/VbanInteractivity.h"

#include "Build.h"

using namespace ::Voicemeeter;

template<
	typename TInput, typename TInputIterator,
	typename TOutput, typename TOutputIterator,
	typename TMapper>
class PlugStatePromotion : public UI::Policies::IStatePromotion<int> {
	static_assert(
		::estd::is_invocable_r<bool, TMapper, const int&>(),
		"TMapper must be invocable with const int& and must return bool");

public:
	PlugStatePromotion(
		IMixer<TInput, TInputIterator, TOutput, TOutputIterator>& mixer,
		const TInput& input,
		const TOutput& output,
		const TMapper& mapper
	) : m_mixer{ mixer }
		, m_input{ input }
		, m_output{ output }
		, m_mapper{ mapper } {

	};
	PlugStatePromotion() = delete;
	PlugStatePromotion(const PlugStatePromotion&) = delete;
	PlugStatePromotion(PlugStatePromotion&&) = delete;

	~PlugStatePromotion() = default;

	PlugStatePromotion& operator=(const PlugStatePromotion&) = delete;
	PlugStatePromotion& operator=(PlugStatePromotion&&) = delete;

	virtual void Promote(const int& state) const {
		m_mixer.set_Plug(m_input, m_output, m_mapper(state));
	};

private:
	IMixer<TInput, TInputIterator, TOutput, TOutputIterator>& m_mixer;
	const TInput& m_input;
	const TOutput& m_output;
	TMapper m_mapper;
};

template<typename TMapper>
class VbanStatePromotion : public UI::Policies::IStatePromotion<int> {
	static_assert(
		::estd::is_invocable_r<bool, TMapper, const int&>(),
		"TMapper must be invocable with const int& and must return bool");

public:
	VbanStatePromotion(
		INetwork& network,
		const TMapper& mapper
	) : m_network{ network }
		, m_mapper{ mapper } {

	};
	VbanStatePromotion() = delete;
	VbanStatePromotion(const VbanStatePromotion&) = delete;
	VbanStatePromotion(VbanStatePromotion&&) = delete;

	~VbanStatePromotion() = default;

	VbanStatePromotion& operator=(const VbanStatePromotion&) = delete;
	VbanStatePromotion& operator=(VbanStatePromotion&&) = delete;

	virtual void Promote(const int& state) const {
		m_network.set_Vban(m_mapper(state));
	};

private:
	INetwork& m_network;
	TMapper m_mapper;
};

template<typename TGainMapper, typename TMutedMapper>
class KnobStatePromotion : public UI::Policies::IStatePromotion<UI::States::Knob> {
	static_assert(
		::estd::is_invocable_r<double, TGainMapper, const int&>(),
		"TGainMapper must be invocable with const int& and must return double");
	static_assert(
		::estd::is_invocable_r<bool, TMutedMapper, const bool&>(),
		"TMutedMapper must be invocable with const bool& and must return bool");

public:
	KnobStatePromotion(
		Decorators::IAmplifier& amplifier,
		const TGainMapper& gainMapper,
		const TMutedMapper& mutedMapper
	) : m_amplifier{ amplifier }
		, m_gainMapper{ gainMapper }
		, m_mutedMapper{ mutedMapper } {

	};
	KnobStatePromotion() = delete;
	KnobStatePromotion(const KnobStatePromotion&) = delete;
	KnobStatePromotion(KnobStatePromotion&&) = delete;

	~KnobStatePromotion() = default;

	KnobStatePromotion& operator=(const KnobStatePromotion&) = delete;
	KnobStatePromotion& operator=(KnobStatePromotion&&) = delete;

	virtual void Promote(const UI::States::Knob& state) const {
		m_amplifier.set_Gain(m_gainMapper(state.gain));
		m_amplifier.set_Mute(m_mutedMapper(state.enabled));
	};

private:
	Decorators::IAmplifier& m_amplifier;
	TGainMapper m_gainMapper;
	TMutedMapper m_mutedMapper;
};

UI::D2D::Scene* Scene::D2D::Remote::Build(
	HWND hWnd,
	::Environment::IInputTracker& inputTracker,
	::Environment::ITimer& timer,
	::Voicemeeter::Remote::Mixer& mixer
) {
	::std::unique_ptr<UI::FocusTracker> pFocusTracker{
		new UI::FocusTracker{}
	};
	::std::unique_ptr<UI::InputTracker> pInputTracker{
		new UI::InputTracker{ inputTracker }
	};

	const UI::D2D::Graphics::Theme theme{ UI::D2D::Graphics::Theme::Default() };
	::std::unique_ptr<UI::D2D::Graphics::Canvas> pCanvas{
		new UI::D2D::Graphics::Canvas{ hWnd, theme }
	};

	auto checkboxMap = [](const int& state)->bool {
		return static_cast<bool>(state);
		};
	auto gainerMap = [](const int& state)->double {
		return (state / 100. - 90.) / 3.75;
		};

	::std::unique_ptr<VbanStatePromotion<decltype(checkboxMap)>> vban_pStatePromotionPolicy{
		new VbanStatePromotion<decltype(checkboxMap)>{ mixer.get_Network(), checkboxMap }
	};
	::std::unique_ptr<UI::D2D::Graphics::Glyphs::Vban> vban_pGlyph{
		new UI::D2D::Graphics::Glyphs::Vban{
			*pCanvas
	} };
	::std::shared_ptr<UI::Policies::CheckboxStateChange> pCheckboxStateChangePolicy{
		new UI::Policies::CheckboxStateChange{}
	};
	::std::shared_ptr<UI::D2D::Decorators::WindowsGlyphUpdate<UI::D2D::Graphics::Glyphs::Vban, int, UI::D2D::Policies::VbanGlyphUpdate>> pVbanGlyphUpdatePolicy{
		new UI::D2D::Decorators::WindowsGlyphUpdate<UI::D2D::Graphics::Glyphs::Vban, int, UI::D2D::Policies::VbanGlyphUpdate>{ hWnd }
	};
	::std::shared_ptr<UI::D2D::Policies::VbanInteractivity> pVbanInteractivityPolicy{
		new UI::D2D::Policies::VbanInteractivity{ *pFocusTracker }
	};

	::std::vector<::std::unique_ptr<UI::IComponent>> cpComponent{};

	::std::unique_ptr<UI::D2D::Controls::Vban> pVban{
		new UI::Decorators::RegionCheck<
			UI::D2D::Controls::Vban>{
				*pInputTracker,
				::std::move(vban_pGlyph),
				pCheckboxStateChangePolicy,
				::std::move(vban_pStatePromotionPolicy),
				pVbanGlyphUpdatePolicy,
				pVbanInteractivityPolicy
	} };

	UI::D2D::Controls::Vban& checkbox{ *pVban };
	mixer.get_Network()
		.on_Vban([&checkbox](bool vban)->void {
		int value{ vban };
		checkbox.Set(value, false);
			});

	cpComponent.push_back(::std::move(pVban));

	::std::shared_ptr<UI::D2D::Policies::KnobStateChange> pKnobStateChangePolicy{
		new UI::D2D::Policies::KnobStateChange{}
	};
	::std::shared_ptr<UI::D2D::Decorators::WindowsGlyphUpdate<UI::D2D::Graphics::Glyphs::Knob, UI::States::Knob, UI::D2D::Policies::KnobGlyphUpdate>> pKnobGlyphUpdatePolicy{
		new UI::D2D::Decorators::WindowsGlyphUpdate<UI::D2D::Graphics::Glyphs::Knob, UI::States::Knob, UI::D2D::Policies::KnobGlyphUpdate>{ hWnd }
	};
	::std::shared_ptr<UI::D2D::Policies::KnobInteractivity> pKnobInteractivityPolicy{
		new UI::D2D::Policies::KnobInteractivity{ *pInputTracker, *pFocusTracker , timer }
	};

	::std::shared_ptr<UI::D2D::Decorators::WindowsGlyphUpdate<UI::D2D::Graphics::Glyphs::Plug, int, UI::D2D::Policies::PlugGlyphUpdate>> pPlugGlyphUpdatePolicy{
		new UI::D2D::Decorators::WindowsGlyphUpdate<UI::D2D::Graphics::Glyphs::Plug, int, UI::D2D::Policies::PlugGlyphUpdate>{ hWnd }
	};
	::std::shared_ptr<UI::D2D::Policies::PlugInteractivity> pPlugInteractivityPolicy{
		new UI::D2D::Policies::PlugInteractivity{ *pFocusTracker }
	};

	for (::Voicemeeter::Remote::Input& input : mixer.get_Inputs()) {
		::std::unique_ptr<UI::D2D::Graphics::Glyphs::Knob> pKnobGlyph{
			new UI::D2D::Graphics::Glyphs::Knob{
				*pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(input.get_Label())
		} };

		::std::unique_ptr<KnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>> pKnobStatePromotionPolicy{
			new KnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>{ input, gainerMap, checkboxMap }
		};

		::std::unique_ptr<UI::D2D::Controls::Knob> pKnob{
			new UI::Decorators::Margin<
				UI::Decorators::RegionCheck<
					UI::D2D::Controls::Knob>>{
						::linear_algebra::vectord{ 2, 0 },
						::linear_algebra::vectord{ 0, 0 },
						* pInputTracker,
						::std::move(pKnobGlyph),
						pKnobStateChangePolicy,
						::std::move(pKnobStatePromotionPolicy),
						pKnobGlyphUpdatePolicy,
						pKnobInteractivityPolicy
		} };

		UI::D2D::Controls::Knob& knob{ *pKnob };
		input.on_Gain([&knob](double gain)->void {
			UI::States::Knob state{ knob.get_State() };
			state.gain = static_cast<int>((gain * 3.75 + 90.) * 100.);
			knob.Set(state, false);
			});
		input.on_Mute([&knob](bool mute)->void {
			UI::States::Knob state{ knob.get_State() };
			state.enabled = mute;
			knob.Set(state, false);
			});

		cpComponent.push_back(::std::move(pKnob));

		::std::vector<::std::unique_ptr<UI::IComponent>> cpPlug{};

		for (::Voicemeeter::Remote::Output& output : mixer.get_Outputs()) {
			::std::unique_ptr<UI::D2D::Graphics::Glyphs::Plug> pPlugGlyph{
				new UI::D2D::Graphics::Glyphs::Plug{
					*pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(output.get_Label())
			} };

			::std::unique_ptr<PlugStatePromotion<
				::Voicemeeter::Remote::Input, ::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Input>,
				::Voicemeeter::Remote::Output, ::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Output>,
				decltype(checkboxMap)>> pPlugStatePromotionPolicy{
				new PlugStatePromotion<
					::Voicemeeter::Remote::Input, ::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Input>,
					::Voicemeeter::Remote::Output, ::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Output>,
					decltype(checkboxMap)>{ mixer, input, output, checkboxMap }
			};

			::std::unique_ptr<UI::D2D::Controls::Plug> pPlug{
				(cpPlug.size()
					? ::std::make_unique<UI::Decorators::Margin<
						UI::Decorators::RegionCheck<
							UI::D2D::Controls::Plug>>>(
								::linear_algebra::vectord{ 0, 2 },
								::linear_algebra::vectord{ 0, 0 },
								*pInputTracker,
								::std::move(pPlugGlyph),
								pCheckboxStateChangePolicy,
								::std::move(pPlugStatePromotionPolicy),
								pPlugGlyphUpdatePolicy,
								pPlugInteractivityPolicy
					) : ::std::make_unique<UI::Decorators::RegionCheck<
						UI::D2D::Controls::Plug>>(
							*pInputTracker,
							::std::move(pPlugGlyph),
							pCheckboxStateChangePolicy,
							::std::move(pPlugStatePromotionPolicy),
							pPlugGlyphUpdatePolicy,
							pPlugInteractivityPolicy
				))
			};

			UI::D2D::Controls::Plug& checkbox{ *pPlug };
			mixer.on_Plug(input, output, [&checkbox](bool plug)->void {
				int value{ plug };
				checkbox.Set(value, false);
				});

			cpPlug.push_back(::std::move(pPlug));

			if (cpPlug.size() == 2) {
				cpComponent.push_back(
					::std::make_unique<UI::Decorators::Margin<
					UI::Decorators::RegionCheck<
					UI::Panels::Stack<UI::Panels::Direction::Down>>>>(
						::linear_algebra::vectord{ 2, 0 },
						::linear_algebra::vectord{ 0, 0 },
						*pInputTracker,
						cpPlug.begin(),
						cpPlug.end()
						)
				);
				cpPlug.clear();
			}
		}
	}

	for (::Voicemeeter::Remote::Output& output : mixer.get_Outputs()) {
		::std::unique_ptr<UI::D2D::Graphics::Glyphs::Knob> pKnobGlyph{
			new UI::D2D::Graphics::Glyphs::Knob{
				*pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(output.get_Label())
		} };

		::std::unique_ptr<KnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>> pKnobStatePromotionPolicy{
			new KnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>{ output, gainerMap, checkboxMap }
		};

		::std::unique_ptr<UI::D2D::Controls::Knob> pKnob{
			new UI::Decorators::Margin<
				UI::Decorators::RegionCheck<
					UI::D2D::Controls::Knob>>{
						::linear_algebra::vectord{ 2, 0 },
						::linear_algebra::vectord{ 0, 0 },
						* pInputTracker,
						::std::move(pKnobGlyph),
						pKnobStateChangePolicy,
						::std::move(pKnobStatePromotionPolicy),
						pKnobGlyphUpdatePolicy,
						pKnobInteractivityPolicy
		} };

		UI::D2D::Controls::Knob& knob{ *pKnob };
		output.on_Gain([&knob](double gain)->void {
			UI::States::Knob state{ knob.get_State() };
			state.gain = static_cast<int>((gain * 3.75 + 90.) * 100.);
			knob.Set(state, false);
			});
		output.on_Mute([&knob](bool mute)->void {
			UI::States::Knob state{ knob.get_State() };
			state.enabled = mute;
			knob.Set(state, false);
			});

		cpComponent.push_back(::std::move(pKnob));
	}

	::std::unique_ptr<UI::IComponent> pComposition{
		new UI::Panels::Stack<UI::Panels::Direction::Right>{
			cpComponent.begin(),
			cpComponent.end()
	} };

	return new UI::D2D::Scene{
		::std::move(pInputTracker),
		::std::move(pFocusTracker),
		::std::move(pCanvas),
		::std::move(pComposition)
	};
}