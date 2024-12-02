#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <codecvt>
#include <locale>
#include <memory>
#include <vector>
#include <utility>

#include "estd/type_traits.h"

#include "Voicemeeter.UI/Decorators/Margin.h"
#include "Voicemeeter.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.UI/Panels/Stack.h"
#include "Voicemeeter.UI/Policies/State/Changes/Checkbox.h"
#include "Voicemeeter.UI/Policies/State/Changes/Knob.h"
#include "Voicemeeter.UI/States/Knob.h"
#include "Voicemeeter.UI/Trackers/Dirty.h"
#include "Voicemeeter.UI/Trackers/Focus.h"
#include "Voicemeeter.UI/Trackers/Input.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Animations/Knob.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Animations/Plug.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Animations/Vban.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Knob.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Plug.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Vban.h"
#include "Voicemeeter.UI.D2D/Controls/Knob.h"
#include "Voicemeeter.UI.D2D/Controls/Plug.h"
#include "Voicemeeter.UI.D2D/Controls/Vban.h"
#include "Voicemeeter.UI.D2D/Decorators/Interactivity/Knob.h"
#include "Voicemeeter.UI.D2D/Decorators/Interactivity/Plug.h"
#include "Voicemeeter.UI.D2D/Decorators/Interactivity/Vban.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Knob.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Plug.h"
#include "Voicemeeter.UI.D2D/Graphics/Glyphs/Vban.h"
#include "Voicemeeter.UI.D2D/Graphics/Canvas.h"
#include "Voicemeeter.UI.D2D/Graphics/Theme.h"
#include "Voicemeeter.UI.D2D/Policies/Glyph/Updates/Knob.h"
#include "Voicemeeter.UI.D2D/Policies/Glyph/Updates/Plug.h"
#include "Voicemeeter.UI.D2D/Policies/Glyph/Updates/VBan.h"
#include "Voicemeeter.UI.D2D/Policies/State/Changes/Knob.h"
#include "Windows/Registry.h"

#include "Build.h"

using namespace ::Voicemeeter;

UI::D2D::Graphics::Theme LoadTheme() {
	UI::D2D::Graphics::Theme theme{ UI::D2D::Graphics::Theme::Default() };
	HKEY hKey{ HKEY_CURRENT_USER };
	::std::wstring subKey{ LR"(SOFTWARE\VoicemeeterDeskBand\Theme)" };
	::Windows::Registry::TryGetValue(hKey, subKey, L"FontFamily", theme.FontFamily);
	DWORD color{};
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"Warning", color)) {
		theme.Warning = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"Danger", color)) {
		theme.Danger = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"DarkGlass", color)) {
		theme.DarkGlass = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"LightGlass", color)) {
		theme.LightGlass = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"PrimaryActive", color)) {
		theme.PrimaryActive = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"SecondaryActive", color)) {
		theme.SecondaryActive = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"Inactive", color)) {
		theme.Inactive = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"Indicator", color)) {
		theme.Indicator = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"EqualizerLow", color)) {
		theme.EqualizerLow = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"EqualizerMedium", color)) {
		theme.EqualizerMedium = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	if (::Windows::Registry::TryGetValue(hKey, subKey, L"EqualizerHigh", color)) {
		theme.EqualizerHigh = ::D2D1::ColorF(static_cast<UINT32>(color));
	}
	return theme;
}

UI::D2D::Scene* Scene::D2D::Remote::Build(
	HWND hWnd,
	UI::Direction direction,
	::Environment::IDirtyTracker& dirtyTracker,
	::Environment::IInputTracker& inputTracker,
	::Environment::ITimer& compositionTimer,
	::Environment::ITimer& dirtyTimer,
	::Voicemeeter::Remote::Mixer& mixer
) {
	DWORD animations{ 1UL };
	::Windows::Registry::TryGetValue(HKEY_CURRENT_USER, LR"(SOFTWARE\VoicemeeterDeskBand)", L"Animations", animations);

	::std::unique_ptr<UI::Trackers::IDirty> pDirtyTracker{
		new UI::Trackers::Dirty{ dirtyTracker, dirtyTimer }
	};
	::std::unique_ptr<UI::Trackers::IFocus> pFocusTracker{
		new UI::Trackers::Focus{}
	};
	::std::unique_ptr<UI::Trackers::IInput> pInputTracker{
		new UI::Trackers::Input{ inputTracker }
	};

	const UI::D2D::Graphics::Theme theme{ LoadTheme() };
	::std::unique_ptr<UI::D2D::Graphics::Canvas> pCanvas{
		new UI::D2D::Graphics::Canvas{ hWnd, theme }
	};

	auto checkboxMap = [](const int& state)->bool {
		return static_cast<bool>(state);
	};
	auto gainerMap = [](const int& state)->double {
		return (state / 100. - 90.) / 3.75;
	};

	::std::vector<::std::unique_ptr<UI::IComponent>> cpComponent{};

	::Remote::Network& network{ mixer.get_Network() };
	if (network.get_Supported()) {
		::std::unique_ptr<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Vban, int>> vban_pGlyph{
			(animations
				? static_cast<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Vban, int>*>(new UI::D2D::Adapters::Glyph::Updates::Animations::Vban{
						*pDirtyTracker, *pCanvas
				}) : new UI::D2D::Adapters::Glyph::Updates::Vban{
						*pCanvas
		}) };
		::std::shared_ptr<UI::D2D::Policies::Glyph::Updates::Vban> pVbanGlyphUpdatePolicy{
			new UI::D2D::Policies::Glyph::Updates::Vban{}
		};

		::std::unique_ptr<UI::D2D::Controls::Vban> pVban{
			new UI::Decorators::RegionCheck<
				UI::D2D::Decorators::Interactivity::Vban<
					UI::D2D::Controls::Vban>>{
						*pInputTracker,
						*pFocusTracker,
						*pDirtyTracker, vban_pGlyph, pCheckboxStateChangePolicy, vban_pStatePromotionPolicy, pVbanGlyphUpdatePolicy
		} };

		UI::D2D::Controls::Vban& checkbox{ *pVban };
		network.on_Vban([&checkbox](bool vban)->void {
			int value{ vban };
			checkbox.Set(value, false);
		});

		cpComponent.push_back(::std::move(pVban));
	}

	::std::shared_ptr<UI::D2D::Policies::State::Changes::Knob> pKnobStateChangePolicy{
		new UI::D2D::Policies::State::Changes::Knob{}
	};
	::std::shared_ptr<UI::D2D::Policies::Glyph::Updates::Knob> pKnobGlyphUpdatePolicy{
		new UI::D2D::Policies::Glyph::Updates::Knob{}
	};
	::std::shared_ptr<UI::D2D::Policies::Glyph::Updates::Plug> pPlugGlyphUpdatePolicy{
		new UI::D2D::Policies::Glyph::Updates::Plug{}
	};

	::std::valarray<double> left_top{
		(direction == UI::Direction::Right
			? ::std::valarray<double>{ 2., 0. }
			: ::std::valarray<double>{ 0., 2. })
	};

	for (::Voicemeeter::Remote::Input& input : mixer.get_Inputs()) {
		::std::unique_ptr<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Knob, UI::States::Knob>> pKnobGlyph{
			(animations
				? static_cast<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Knob, UI::States::Knob>*>(new UI::D2D::Adapters::Glyph::Updates::Animations::Knob{
					*pDirtyTracker, *pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(input.get_Label())
				}) : new UI::D2D::Adapters::Glyph::Updates::Knob{
					*pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(input.get_Label())
		}) };

		::std::unique_ptr<UI::Policies::State::IPromotion<UI::States::Knob>> pKnobStatePromotionPolicy{
			new KnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>{ input, gainerMap, checkboxMap }
		};

		::std::unique_ptr<UI::D2D::Controls::Knob> pKnob{ nullptr };
		if (cpComponent.empty()) {
			pKnob.reset(
				(direction == UI::Direction::Right
					? static_cast<UI::D2D::Controls::Knob*>(new UI::Decorators::RegionCheck<
						UI::D2D::Decorators::Interactivity::Knob<
							UI::Direction::Right, UI::D2D::Controls::Knob>>{
								*pInputTracker,
								*pInputTracker, *pFocusTracker, compositionTimer,
								*pDirtyTracker, pKnobGlyph, pKnobStateChangePolicy, pKnobStatePromotionPolicy, pKnobGlyphUpdatePolicy
					}) : new UI::Decorators::RegionCheck<
						UI::D2D::Decorators::Interactivity::Knob<
							UI::Direction::Down, UI::D2D::Controls::Knob>>{
								*pInputTracker,
								*pInputTracker, *pFocusTracker, compositionTimer,
								*pDirtyTracker, pKnobGlyph, pKnobStateChangePolicy, pKnobStatePromotionPolicy, pKnobGlyphUpdatePolicy
					} ));
		} else {
			pKnob.reset(
				(direction == UI::Direction::Right
					? static_cast<UI::D2D::Controls::Knob*>(new UI::Decorators::Margin<
						UI::Decorators::RegionCheck<
							UI::D2D::Decorators::Interactivity::Knob<
								UI::Direction::Right, UI::D2D::Controls::Knob>>>{
									left_top, ::std::valarray<double>{ 0., 0. },
									*pInputTracker,
									*pInputTracker, *pFocusTracker, compositionTimer,
									*pDirtyTracker, pKnobGlyph, pKnobStateChangePolicy, pKnobStatePromotionPolicy, pKnobGlyphUpdatePolicy
					}) : new UI::Decorators::Margin<
						UI::Decorators::RegionCheck<
							UI::D2D::Decorators::Interactivity::Knob<
								UI::Direction::Down, UI::D2D::Controls::Knob>>>{
									left_top, ::std::valarray<double>{ 0., 0. },
									*pInputTracker,
									*pInputTracker, *pFocusTracker, compositionTimer,
									*pDirtyTracker, pKnobGlyph, pKnobStateChangePolicy, pKnobStatePromotionPolicy, pKnobGlyphUpdatePolicy
					} ));
		}


		UI::D2D::Controls::Knob& knob{ *pKnob };
		input.on_Gain([&knob](double gain)->void {
			UI::States::Knob state{ knob.get_State() };
			state.gain = static_cast<int>((gain * 3.75 + 90.) * 100.);
			knob.Set(state, false);
		});
		input.on_Mute([&knob](bool mute)->void {
			UI::States::Knob state{ knob.get_State() };
			state.toggle = mute;
			knob.Set(state, false);
		});
		for (::Voicemeeter::Remote::Channel& channel : input.get_Channels()) {
			UI::States::Knob state{ knob.get_State() };
			size_t i{ state.level.size() };
			state.level.resize(state.level.size() + 1);
			knob.Set(state, false);

			channel.on_Level([&knob, i](double level)->void {
				UI::States::Knob state{ knob.get_State() };
				state.level[i] = static_cast<int>(level * 10000.);
				knob.Set(state, false);
			});
		}

		cpComponent.push_back(::std::move(pKnob));

		::std::vector<::std::unique_ptr<UI::IComponent>> cpPlug{};

		for (::Voicemeeter::Remote::Output& output : mixer.get_Outputs()) {
			::std::unique_ptr<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Plug, int>> pPlugGlyph{
				(animations
					? static_cast<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Plug, int>*>(new UI::D2D::Adapters::Glyph::Updates::Animations::Plug{
						*pDirtyTracker, *pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(output.get_Label())
					}) : new UI::D2D::Adapters::Glyph::Updates::Plug{
						*pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(output.get_Label())
			}) };

			::std::unique_ptr<UI::Policies::State::IPromotion<int>> pPlugStatePromotionPolicy{
				new PlugStatePromotion<
					::Voicemeeter::Remote::Input, ::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Input>,
					::Voicemeeter::Remote::Output, ::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Output>,
					decltype(checkboxMap)>{ mixer, input, output, checkboxMap }
			};

			::std::unique_ptr<UI::D2D::Controls::Plug> pPlug{
				(cpPlug.size()
					? ::std::make_unique<UI::Decorators::Margin<
						UI::Decorators::RegionCheck<
							UI::D2D::Decorators::Interactivity::Plug<
								UI::D2D::Controls::Plug>>>>(
									::std::valarray<double>{ 0., 2. },
									::std::valarray<double>{ 0., 0. },
									*pInputTracker,
									*pFocusTracker,
									*pDirtyTracker, pPlugGlyph, pCheckboxStateChangePolicy, pPlugStatePromotionPolicy, pPlugGlyphUpdatePolicy
					) : ::std::make_unique<UI::Decorators::RegionCheck<
						UI::D2D::Decorators::Interactivity::Plug<
							UI::D2D::Controls::Plug>>>(
								*pInputTracker,
								*pFocusTracker,
								*pDirtyTracker, pPlugGlyph, pCheckboxStateChangePolicy, pPlugStatePromotionPolicy, pPlugGlyphUpdatePolicy
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
							UI::Panels::Stack<UI::Direction::Down>>>>(
								left_top, ::std::valarray<double>{ 0., 0. },
								*pInputTracker,
								cpPlug.begin(), cpPlug.end()
							)
				);
				cpPlug.clear();
			}
		}
	}

	for (::Voicemeeter::Remote::Output& output : mixer.get_Outputs()) {
		::std::unique_ptr<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Knob, UI::States::Knob>> pKnobGlyph{
			(animations
				? static_cast<UI::D2D::Adapters::Glyph::IUpdate<UI::D2D::Graphics::Glyphs::Knob, UI::States::Knob>*>(new UI::D2D::Adapters::Glyph::Updates::Animations::Knob{
					*pDirtyTracker, *pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(output.get_Label())
				}) : new UI::D2D::Adapters::Glyph::Updates::Knob{
					*pCanvas, ::std::wstring_convert<::std::codecvt_utf8<wchar_t>>().from_bytes(output.get_Label())
		}) };

		::std::unique_ptr<UI::Policies::State::IPromotion<UI::States::Knob>> pKnobStatePromotionPolicy{
			new KnobStatePromotion<decltype(gainerMap), decltype(checkboxMap)>{ output, gainerMap, checkboxMap }
		};

		::std::unique_ptr<UI::D2D::Controls::Knob> pKnob{
			(direction == UI::Direction::Right
				? static_cast<UI::D2D::Controls::Knob*>(new UI::Decorators::Margin<
					UI::Decorators::RegionCheck<
						UI::D2D::Decorators::Interactivity::Knob<
							UI::Direction::Right, UI::D2D::Controls::Knob>>>{
								left_top, ::std::valarray<double>{ 0., 0. },
								*pInputTracker,
								*pInputTracker, *pFocusTracker, compositionTimer,
								*pDirtyTracker, pKnobGlyph, pKnobStateChangePolicy, pKnobStatePromotionPolicy, pKnobGlyphUpdatePolicy
				}) : new UI::Decorators::Margin<
					UI::Decorators::RegionCheck<
						UI::D2D::Decorators::Interactivity::Knob<
							UI::Direction::Down, UI::D2D::Controls::Knob>>>{
								left_top, ::std::valarray<double>{ 0., 0. },
								*pInputTracker,
								*pInputTracker, *pFocusTracker, compositionTimer,
								*pDirtyTracker, pKnobGlyph, pKnobStateChangePolicy, pKnobStatePromotionPolicy, pKnobGlyphUpdatePolicy
				} )};

		UI::D2D::Controls::Knob& knob{ *pKnob };
		output.on_Gain([&knob](double gain)->void {
			UI::States::Knob state{ knob.get_State() };
			state.gain = static_cast<int>((gain * 3.75 + 90.) * 100.);
			knob.Set(state, false);
		});
		output.on_Mute([&knob](bool mute)->void {
			UI::States::Knob state{ knob.get_State() };
			state.toggle = mute;
			knob.Set(state, false);
		});
		for (::Voicemeeter::Remote::Channel& channel : output.get_Channels()) {
			UI::States::Knob state{ knob.get_State() };
			size_t i{ state.level.size() };
			state.level.resize(state.level.size() + 1);
			knob.Set(state, false);

			channel.on_Level([&knob, i](double level)->void {
				UI::States::Knob state{ knob.get_State() };
				state.level[i] = static_cast<int>(level * 10000.);
				knob.Set(state, false);
			});
		}

		cpComponent.push_back(::std::move(pKnob));
	}

	::std::unique_ptr<UI::IComponent> pComposition{
		(direction == UI::Direction::Right
			? static_cast<UI::IComponent*>(new UI::Panels::Stack<UI::Direction::Right>{
				cpComponent.begin(), cpComponent.end()
			}) : new UI::Panels::Stack<UI::Direction::Down>{
				cpComponent.begin(), cpComponent.end()
			})
	};

	return new UI::D2D::Scene{
		pDirtyTracker, pInputTracker, pFocusTracker, pCanvas, pComposition
	};
}