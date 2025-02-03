#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX
#define _USE_MATH_DEFINES

#include <codecvt>
#include <exception>
#include <locale>
#include <memory>
#include <vector>
#include <utility>

#include "Voicemeeter.UI/Decorators/Margin.h"
#include "Voicemeeter.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.UI/Panels/Stack.h"
#include "Voicemeeter.UI/Policies/State/Changes/Checkbox.h"
#include "Voicemeeter.UI.D3D12/Decorators/Bundle/Animations/Knob.h"
#include "Voicemeeter.UI.D3D12/Decorators/Bundle/Animations/Plug.h"
#include "Voicemeeter.UI.D3D12/Decorators/Bundle/Animations/Vban.h"
#include "Voicemeeter.UI.D3D12/Graphics/Bundles/Knob.h"
#include "Voicemeeter.UI.D3D12/Graphics/Bundles/Plug.h"
#include "Voicemeeter.UI.D3D12/Graphics/Bundles/Vban.h"
#include "Voicemeeter.UI.D3D12/Graphics/Glyph.h"
#include "Voicemeeter.UI.D3D12/Policies/Glyph/Updates/Animations/Knob.h"
#include "Voicemeeter.UI.D3D12/Policies/Glyph/Updates/Animations/Plug.h"
#include "Voicemeeter.UI.D3D12/Policies/Glyph/Updates/Animations/Vban.h"
#include "Voicemeeter.UI.D3D12/Policies/Glyph/Updates/Knob.h"
#include "Voicemeeter.UI.D3D12/Policies/Glyph/Updates/Plug.h"
#include "Voicemeeter.UI.D3D12/Policies/Glyph/Updates/Vban.h"
#include "Voicemeeter.UI.Cherry/Decorators/Interactivity/Knob.h"
#include "Voicemeeter.UI.Cherry/Decorators/Interactivity/Plug.h"
#include "Voicemeeter.UI.Cherry/Decorators/Interactivity/Vban.h"
#include "Voicemeeter.UI.Cherry/Policies/State/Changes/Knob.h"

#include "Cherry.h"

using namespace ::Voicemeeter::Clients::UI::D3D12;

template<::Voicemeeter::UI::Direction Direction>
inline static ::std::valarray<double> MarginPosition();
template<>
inline static ::std::valarray<double> MarginPosition<::Voicemeeter::UI::Direction::Right>() {
	return ::std::valarray<double>{ 2., 0. };
}
template<>
inline static ::std::valarray<double> MarginPosition<::Voicemeeter::UI::Direction::Down>() {
	return ::std::valarray<double>{ 0., 2. };
}

template<typename TGlyph, typename TGlyphUpdate>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeVban(
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::std::unique_ptr<TGlyph>& pGlyph,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	auto changeNotify = [&mixer](const int& state)->void {
		mixer.set_Vban<Cherry>(state);
	};
	using Control = ::Voicemeeter::UI::Decorators::RegionCheck<
		::Voicemeeter::UI::Cherry::Decorators::Interactivity::Vban<
			TGlyph, decltype(changeNotify), TGlyphUpdate,
			::Voicemeeter::UI::Cherry::Controls::Vban<
				TGlyph, decltype(changeNotify), TGlyphUpdate>>>;
	::std::unique_ptr<Control> pControl{
		new Control{
			inputTracker,
			focusTracker,
			pGlyph->get_BaseSize(), pGlyph, changeNotify
		}
	};
	subscription.on_Vban(
		[&control = *pControl](bool value)->void {
			int src{ value };
			control.Set(src, false);
		});
	return pControl;
}
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeVban(
	bool animations,
	::Voicemeeter::UI::D3D12::Graphics::Palette& palette,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	using Bundle = ::Voicemeeter::UI::D3D12::Graphics::Bundles::Vban;
	if (animations) {
		using Animation = ::Voicemeeter::UI::D3D12::Decorators::Bundle::Animations::Vban<
			Bundle>;
		using Glyph = ::Voicemeeter::UI::D3D12::Graphics::Glyph<
			Animation>;
		using GlyphUpdate = ::Voicemeeter::UI::D3D12::Policies::Glyph::Updates::Animations::Vban<
			Bundle, Animation, Glyph>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette } };
		return ComposeVban<Glyph, GlyphUpdate>(
			focusTracker, inputTracker,
			pGlyph,
			mixer, subscription);
	} else {
		using Glyph = ::Voicemeeter::UI::D3D12::Graphics::Glyph<
			Bundle>;
		using GlyphUpdate = ::Voicemeeter::UI::D3D12::Policies::Glyph::Updates::Vban<
			Bundle, Glyph>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette } };
		return ComposeVban<Glyph, GlyphUpdate>(
			focusTracker, inputTracker,
			pGlyph,
			mixer, subscription);
	}
}

template<::Voicemeeter::UI::Direction Direction, ::Voicemeeter::UI::Direction MarginDirection, typename TGlyph, typename TGlyphUpdate, typename TStrip>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
	bool first,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Environment::ITimer& timer,
	::std::unique_ptr<TGlyph>& pGlyph,
	TStrip& strip,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	auto changeNotify = [&strip](const ::Voicemeeter::UI::Cherry::States::Knob& state)->void {
		strip.set_Gain<Cherry>(state.gain / 375.);
		strip.set_Mute<Cherry>(state.toggle);
	};
	using Scale = ::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio;
	using Control = ::Voicemeeter::UI::Decorators::RegionCheck<
		::Voicemeeter::UI::Cherry::Decorators::Interactivity::Knob<
			Direction, TGlyph, decltype(changeNotify), TGlyphUpdate,
			::Voicemeeter::UI::Cherry::Controls::Knob<
				TGlyph, decltype(changeNotify), TGlyphUpdate>>>;
	::std::unique_ptr<Control> pControl{
		(first
			? new Control{
				inputTracker,
				inputTracker, focusTracker, timer,
				pGlyph->get_BaseSize(), pGlyph, changeNotify
			} : new ::Voicemeeter::UI::Decorators::Margin<Control, Scale>{
				MarginPosition<MarginDirection>(), ::std::valarray<double>{ 0., 0. }, Scale{},
				inputTracker,
				inputTracker, focusTracker, timer,
				pGlyph->get_BaseSize(), pGlyph, changeNotify
			})
	};
	subscription.on_Gain(strip.get_Id(),
		[&control = *pControl](double value)->void {
			::Voicemeeter::UI::Cherry::States::Knob state{ control.get_State() };
			state.gain = static_cast<int>(value * 375.);
			control.Set(state, false);
		});
	subscription.on_Mute(strip.get_Id(),
		[&control = *pControl](bool value)->void {
			::Voicemeeter::UI::Cherry::States::Knob state{ control.get_State() };
			state.toggle = value;
			control.Set(state, false);
		});
	::Voicemeeter::UI::Cherry::States::Knob state{ pControl->get_State() };
	state.id = strip.get_Id();
	state.level.resize(strip.end() - strip.begin());
	pControl->Set(state, false);
	for (auto line = strip.begin(); line != strip.end(); ++line) {
		subscription.on_Level(line->get_Id(),
			[&control = *pControl, i = line - strip.begin()](double value)->void {
				::Voicemeeter::UI::Cherry::States::Knob state{ control.get_State() };
				state.level[i] = static_cast<int>(value * 10000.);
				control.Set(state, false);
			});
	}
	return pControl;
}
template<::Voicemeeter::UI::Direction Direction, typename TGlyph, typename TGlyphUpdate, typename TStrip>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
	::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Environment::ITimer& timer,
	::std::unique_ptr<TGlyph>& pGlyph,
	TStrip& strip,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	switch (marginDirection) {
	case ::Voicemeeter::UI::Direction::Right:
		return ComposeKnob<Direction, ::Voicemeeter::UI::Direction::Right, TGlyph, TGlyphUpdate, TStrip>(
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposeKnob<Direction, ::Voicemeeter::UI::Direction::Down, TGlyph, TGlyphUpdate, TStrip>(
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}
template<typename TGlyph, typename TGlyphUpdate, typename TStrip>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
	::Voicemeeter::UI::Direction direction, ::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Environment::ITimer& timer,
	::std::unique_ptr<TGlyph>& pGlyph,
	TStrip& strip,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	switch (direction) {
	case ::Voicemeeter::UI::Direction::Right:
		return ComposeKnob<::Voicemeeter::UI::Direction::Right, TGlyph, TGlyphUpdate, TStrip>(
			marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposeKnob<::Voicemeeter::UI::Direction::Down, TGlyph, TGlyphUpdate, TStrip>(
			marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}
template<typename TStrip>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
	bool animations, 
	::Voicemeeter::UI::Direction direction, ::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::Voicemeeter::UI::D3D12::Graphics::Palette& palette,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Environment::ITimer& timer,
	TStrip& strip,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	size_t label{ ::Voicemeeter::UI::Cherry::Policies::State::Changes::Knob::ToLabel(strip.get_Id()) };
	using Bundle = ::Voicemeeter::UI::D3D12::Graphics::Bundles::Knob;
	if (animations) {
		using Animation = ::Voicemeeter::UI::D3D12::Decorators::Bundle::Animations::Knob<
			Bundle>;
		using Glyph = ::Voicemeeter::UI::D3D12::Graphics::Glyph<
			Animation>;
		using GlyphUpdate = ::Voicemeeter::UI::D3D12::Policies::Glyph::Updates::Animations::Knob<
			Bundle, Animation, Glyph>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label }};
		return ComposeKnob<Glyph, GlyphUpdate, TStrip>(
			direction, marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription);
	} else {
		using Glyph = ::Voicemeeter::UI::D3D12::Graphics::Glyph<
			Bundle>;
		using GlyphUpdate = ::Voicemeeter::UI::D3D12::Policies::Glyph::Updates::Knob<
			Bundle, Glyph>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label } };
		return ComposeKnob<Glyph, GlyphUpdate, TStrip>(
			direction, marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription);
	}
}

template<::Voicemeeter::UI::Direction MarginDirection, typename TGlyph, typename TGlyphUpdate, typename TInput, typename TOutput>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePlug(
	bool first,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::std::unique_ptr<TGlyph>& pGlyph,
	TInput& input, TOutput& output,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	auto changeNotify = [&input, &output, &mixer](const int& state)->void {
		mixer.set_Plug<Cherry>(input, output, state);
	};
	using Scale = ::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio;
	using Control = ::Voicemeeter::UI::Decorators::RegionCheck<
		::Voicemeeter::UI::Cherry::Decorators::Interactivity::Plug<
			TGlyph, decltype(changeNotify), TGlyphUpdate,
			::Voicemeeter::UI::Cherry::Controls::Plug<
				TGlyph, decltype(changeNotify), TGlyphUpdate>>>;
	::std::unique_ptr<Control> pControl{
		(first
			? new Control{
				inputTracker,
				focusTracker,
				pGlyph->get_BaseSize(), pGlyph, changeNotify
			} : new ::Voicemeeter::UI::Decorators::Margin<Control, Scale>{
				MarginPosition<MarginDirection>(), ::std::valarray<double>{ 0., 0. }, Scale{},
				inputTracker,
				focusTracker,
				pGlyph->get_BaseSize(), pGlyph, changeNotify
			})
	};
	subscription.on_Plug(input.get_Id(), output.get_Id(),
		[&control = *pControl](bool value)->void {
			int state{ value };
			control.Set(state, false);
		});
	return pControl;
}
template<typename TGlyph, typename TGlyphUpdate, typename TInput, typename TOutput>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePlug(
	::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::std::unique_ptr<TGlyph>& pGlyph,
	TInput& input, TOutput& output,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	switch (marginDirection) {
	case ::Voicemeeter::UI::Direction::Right:
		return ComposePlug<::Voicemeeter::UI::Direction::Right, TGlyph, TGlyphUpdate, TInput, TOutput>(
			first,
			focusTracker, inputTracker,
			pGlyph,
			input, output, mixer, subscription
		);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposePlug<::Voicemeeter::UI::Direction::Down, TGlyph, TGlyphUpdate, TInput, TOutput>(
			first,
			focusTracker, inputTracker,
			pGlyph,
			input, output, mixer, subscription
		);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}
template<typename TInput, typename TOutput>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePlug(
	bool animations,
	::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::Voicemeeter::UI::D3D12::Graphics::Palette& palette,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	TInput& input, TOutput& output,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	size_t label{ ::Voicemeeter::UI::Cherry::Policies::State::Changes::Knob::ToLabel(output.get_Id()) };
	using Bundle = ::Voicemeeter::UI::D3D12::Graphics::Bundles::Plug;
	if (animations) {
		using Animation = ::Voicemeeter::UI::D3D12::Decorators::Bundle::Animations::Plug<
			Bundle>;
		using Glyph = ::Voicemeeter::UI::D3D12::Graphics::Glyph<
			Animation>;
		using GlyphUpdate = ::Voicemeeter::UI::D3D12::Policies::Glyph::Updates::Animations::Plug<
			Bundle, Animation, Glyph>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label } };
		return ComposePlug<Glyph, GlyphUpdate, TInput, TOutput>(
			marginDirection,
			first,
			focusTracker, inputTracker,
			pGlyph,
			input, output, mixer, subscription
		);
	} else {
		using Glyph = ::Voicemeeter::UI::D3D12::Graphics::Glyph<
			Bundle>;
		using GlyphUpdate = ::Voicemeeter::UI::D3D12::Policies::Glyph::Updates::Plug<
			Bundle, Glyph>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label } };
		return ComposePlug<Glyph, GlyphUpdate, TInput, TOutput>(
			marginDirection,
			first,
			focusTracker, inputTracker,
			pGlyph,
			input, output, mixer, subscription
		);
	}
}

template<::Voicemeeter::UI::Direction Direction, ::Voicemeeter::UI::Direction MarginDirection>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
	bool first,
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>>& cpComponent
) {
	using Scale = ::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio;
	return ::std::unique_ptr<::Voicemeeter::UI::IComponent>{
		(first
			? new ::Voicemeeter::UI::Panels::Stack<Direction, Scale>{
				cpComponent.begin(), cpComponent.end()
			} : new ::Voicemeeter::UI::Decorators::Margin<
				::Voicemeeter::UI::Panels::Stack<Direction, Scale>, Scale>{
					MarginPosition<MarginDirection>(), ::std::valarray<double>{ 0., 0. }, Scale{},
					cpComponent.begin(), cpComponent.end()
			})
	};
}
template<::Voicemeeter::UI::Direction Direction>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
	::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>>& cpComponent
) {
	switch (marginDirection) {
	case ::Voicemeeter::UI::Direction::Right:
		return ComposePanel<Direction, ::Voicemeeter::UI::Direction::Right>(first, cpComponent);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposePanel<Direction, ::Voicemeeter::UI::Direction::Down>(first, cpComponent);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
	::Voicemeeter::UI::Direction direction, ::Voicemeeter::UI::Direction marginDirection,
	bool first,
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>>& cpComponent
) {
	switch (direction) {
	case ::Voicemeeter::UI::Direction::Right:
		return ComposePanel<::Voicemeeter::UI::Direction::Right>(marginDirection, first, cpComponent);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposePanel<::Voicemeeter::UI::Direction::Down>(marginDirection, first, cpComponent);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}
template<::Voicemeeter::UI::Direction Direction>
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
	const ::std::valarray<double>& marginPoint, const ::std::valarray<double>& marginVertex,
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>>& cpComponent
) {
	using Scale = ::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio;
	return ::std::unique_ptr<::Voicemeeter::UI::IComponent>{
		new ::Voicemeeter::UI::Decorators::Margin<
			::Voicemeeter::UI::Panels::Stack<Direction, Scale>, Scale>{
				marginPoint, marginVertex, Scale{},
				cpComponent.begin(), cpComponent.end()
			}
	};
}
inline static ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
	::Voicemeeter::UI::Direction direction,
	const ::std::valarray<double>& marginPoint, const ::std::valarray<double>& marginVertex,
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>>& cpComponent
) {
	switch (direction) {
	case ::Voicemeeter::UI::Direction::Right:
		return ComposePanel<::Voicemeeter::UI::Direction::Right>(marginPoint, marginVertex, cpComponent);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposePanel<::Voicemeeter::UI::Direction::Down>(marginPoint, marginVertex, cpComponent);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}

template<>
::std::unique_ptr<::Voicemeeter::UI::IComponent> Cherry::Compose(
	::Voicemeeter::UI::D3D12::Graphics::Palette& palette,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker
) {
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription = m_mixer.get_Subscription<Cherry>();
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpComponent{};
	if (m_network) {
		cpComponent.push_back(::std::move(
			ComposeVban(
				m_animations,
				palette,
				focusTracker, inputTracker,
				m_mixer, subscription)));
	}
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpBusComponent{};
	for (auto& input : m_mixer.get_PhysicalInput()) {
		if (m_cIgnoredStrip.find(input.get_Id()) != m_cIgnoredStrip.end()) {
			continue;
		}
		cpBusComponent.push_back(::std::move(
			ComposeKnob(
				m_animations,
				m_direction, m_direction,
				cpBusComponent.empty(),
				palette,
				focusTracker, inputTracker,
				m_compositionTimer,
				input, m_mixer, subscription)));
		::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpPlugComponent{};
		for (auto& output : m_mixer.get_PhysicalOutput()) {
			if (m_cIgnoredStrip.find(output.get_Id()) != m_cIgnoredStrip.end()) {
				continue;
			}
			cpPlugComponent.push_back(::std::move(
				ComposePlug(
					m_animations,
					::Voicemeeter::UI::Direction::Down,
					cpPlugComponent.empty(),
					palette,
					focusTracker, inputTracker,
					input, output, m_mixer, subscription)));
			if (cpPlugComponent.size() == 2) {
				cpBusComponent.push_back(::std::move(
					ComposePanel(
						::Voicemeeter::UI::Direction::Down, m_direction,
						cpBusComponent.empty(),
						cpPlugComponent)));
				cpPlugComponent.clear();
			}
		}
		for (auto& output : m_mixer.get_VirtualOutput()) {
			if (m_cIgnoredStrip.find(output.get_Id()) != m_cIgnoredStrip.end()) {
				continue;
			}
			cpPlugComponent.push_back(::std::move(
				ComposePlug(
					m_animations,
					::Voicemeeter::UI::Direction::Down,
					cpPlugComponent.empty(),
					palette,
					focusTracker, inputTracker,
					input, output, m_mixer, subscription)));
			if (cpPlugComponent.size() == 2) {
				cpBusComponent.push_back(::std::move(
					ComposePanel(
						::Voicemeeter::UI::Direction::Down, m_direction,
						cpBusComponent.empty(),
						cpPlugComponent)));
				cpPlugComponent.clear();
			}
		}
		if (cpPlugComponent.size()) {
			throw ::std::exception{ "Invalid layout" };
		}
	}
	if (cpBusComponent.size()) {
		cpComponent.push_back(::std::move(
			ComposePanel(
				m_direction, m_direction,
				cpComponent.empty(),
				cpBusComponent)));
		cpBusComponent.clear();
	}
	for (auto& input : m_mixer.get_VirtualInput()) {
		if (m_cIgnoredStrip.find(input.get_Id()) != m_cIgnoredStrip.end()) {
			continue;
		}
		cpBusComponent.push_back(::std::move(
			ComposeKnob(
				m_animations,
				m_direction, m_direction,
				cpBusComponent.empty(),
				palette,
				focusTracker, inputTracker,
				m_compositionTimer,
				input, m_mixer, subscription)));
		::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpPlugComponent{};
		for (auto& output : m_mixer.get_PhysicalOutput()) {
			if (m_cIgnoredStrip.find(output.get_Id()) != m_cIgnoredStrip.end()) {
				continue;
			}
			cpPlugComponent.push_back(::std::move(
				ComposePlug(
					m_animations,
					::Voicemeeter::UI::Direction::Down,
					cpPlugComponent.empty(),
					palette,
					focusTracker, inputTracker,
					input, output, m_mixer, subscription)));
			if (cpPlugComponent.size() == 2) {
				cpBusComponent.push_back(::std::move(
					ComposePanel(
						::Voicemeeter::UI::Direction::Down, m_direction,
						cpBusComponent.empty(),
						cpPlugComponent)));
				cpPlugComponent.clear();
			}
		}
		for (auto& output : m_mixer.get_VirtualOutput()) {
			if (m_cIgnoredStrip.find(output.get_Id()) != m_cIgnoredStrip.end()) {
				continue;
			}
			cpPlugComponent.push_back(::std::move(
				ComposePlug(
					m_animations,
					::Voicemeeter::UI::Direction::Down,
					cpPlugComponent.empty(),
					palette,
					focusTracker, inputTracker,
					input, output, m_mixer, subscription)));
			if (cpPlugComponent.size() == 2) {
				cpBusComponent.push_back(::std::move(
					ComposePanel(
						::Voicemeeter::UI::Direction::Down, m_direction,
						cpBusComponent.empty(),
						cpPlugComponent)));
				cpPlugComponent.clear();
			}
		}
		if (cpPlugComponent.size()) {
			throw ::std::exception{ "Invalid layout" };
		}
	}
	if (cpBusComponent.size()) {
		cpComponent.push_back(::std::move(
			ComposePanel(
				m_direction, m_direction,
				cpComponent.empty(),
				cpBusComponent)));
		cpBusComponent.clear();
	}
	for (auto& input : m_mixer.get_PhysicalOutput()) {
		if (m_cIgnoredStrip.find(input.get_Id()) != m_cIgnoredStrip.end()) {
			continue;
		}
		cpBusComponent.push_back(::std::move(
			ComposeKnob(
				m_animations,
				m_direction, m_direction,
				cpBusComponent.empty(),
				palette,
				focusTracker, inputTracker,
				m_compositionTimer,
				input, m_mixer, subscription)));
	}
	for (auto& input : m_mixer.get_VirtualOutput()) {
		if (m_cIgnoredStrip.find(input.get_Id()) != m_cIgnoredStrip.end()) {
			continue;
		}
		cpBusComponent.push_back(::std::move(
			ComposeKnob(
				m_animations,
				m_direction, m_direction,
				cpBusComponent.empty(),
				palette,
				focusTracker, inputTracker,
				m_compositionTimer,
				input, m_mixer, subscription)));
	}
	if (cpBusComponent.size()) {
		cpComponent.push_back(::std::move(
			ComposePanel(
				m_direction, m_direction,
				cpComponent.empty(),
				cpBusComponent)));
		cpBusComponent.clear();
	}
	return (0. < m_marginPoint).max() || (0. < m_marginVertex).max()
		? ComposePanel(
			m_direction,
			m_marginPoint, m_marginVertex,
			cpComponent)
		: ComposePanel(
			m_direction, m_direction,
			true,
			cpComponent);
}