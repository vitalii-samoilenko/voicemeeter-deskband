#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

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
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Animations/Knob.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Animations/Plug.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Animations/Vban.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Knob.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Plug.h"
#include "Voicemeeter.UI.D2D/Adapters/Glyph/Updates/Vban.h"
#include "Voicemeeter.UI.D2D/Decorators/Interactivity/Knob.h"
#include "Voicemeeter.UI.D2D/Decorators/Interactivity/Plug.h"
#include "Voicemeeter.UI.D2D/Decorators/Interactivity/Vban.h"
#include "Voicemeeter.UI.D2D/Policies/State/Changes/Knob.h"
#include "Windows/Registry.h"

#include "Cherry.h"

using namespace ::Voicemeeter::Clients::UI::D2D;

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

inline static ::std::wstring ToLabel(size_t id) {
	switch (id) {
	case 0:
		return L"P";
	case 1:
		return L"V";
	case 2:
		return L"A1";
	case 3:
		return L"A2";
	case 4:
		return L"B1";
	case 5:
		return L"B2";
	default:
		throw ::std::exception{ "Strip is not supported" };
	}
}

template<typename TGlyph>
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
		::Voicemeeter::UI::D2D::Decorators::Interactivity::Vban<TGlyph, decltype(changeNotify)>>;
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
	::Voicemeeter::UI::D2D::Trackers::Dirty& dirtyTracker,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Voicemeeter::UI::D2D::Graphics::Canvas& canvas,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	if (animations) {
		using Glyph = ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Animations::Vban<
			::Voicemeeter::UI::D2D::Graphics::Glyphs::Vban>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ canvas, dirtyTracker } };
		return ComposeVban(
			focusTracker, inputTracker,
			pGlyph,
			mixer, subscription);
	} else {
		using Glyph = ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Vban<
			::Voicemeeter::UI::D2D::Graphics::Glyphs::Vban>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ canvas, dirtyTracker } };
		return ComposeVban(
			focusTracker, inputTracker,
			pGlyph,
			mixer, subscription);
	}
}

template<::Voicemeeter::UI::Direction Direction, ::Voicemeeter::UI::Direction MarginDirection, typename TGlyph, typename TStrip>
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
	auto changeNotify = [&strip](const ::Voicemeeter::UI::States::Knob& state)->void {
		strip.set_Gain<Cherry>((state.gain / 100. - 90.) / 3.75);
		strip.set_Mute<Cherry>(state.toggle);
	};
	using Scale = ::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio;
	using Control = ::Voicemeeter::UI::Decorators::RegionCheck<
		::Voicemeeter::UI::D2D::Decorators::Interactivity::Knob<Direction, TGlyph, decltype(changeNotify)>>;
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
			::Voicemeeter::UI::States::Knob state{ control.get_State() };
			state.gain = static_cast<int>((value * 3.75 + 90.) * 100.);
			control.Set(state, false);
		});
	subscription.on_Mute(strip.get_Id(),
		[&control = *pControl](bool value)->void {
			::Voicemeeter::UI::States::Knob state{ control.get_State() };
			state.toggle = value;
			control.Set(state, false);
		});
	::Voicemeeter::UI::States::Knob state{ pControl->get_State() };
	state.level.resize(strip.end() - strip.begin());
	pControl->Set(state, false);
	for (auto line = strip.begin(); line != strip.end(); ++line) {
		subscription.on_Level(line->get_Id(),
			[&control = *pControl, i = line - strip.begin()](double value)->void {
				::Voicemeeter::UI::States::Knob state{ control.get_State() };
				state.level[i] = static_cast<int>(value * 10000.);
				control.Set(state, false);
			});
	}
	return pControl;
}
template<::Voicemeeter::UI::Direction Direction, typename TGlyph, typename TStrip>
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
		return ComposeKnob<Direction, ::Voicemeeter::UI::Direction::Right>(
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposeKnob<Direction, ::Voicemeeter::UI::Direction::Right>(
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	default:
		throw ::std::exception{ "Direction is not supported" };
	}
}
template<typename TGlyph, typename TStrip>
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
		return ComposeKnob<::Voicemeeter::UI::Direction::Right>(
			marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription
		);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposeKnob<::Voicemeeter::UI::Direction::Down>(
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
	::Voicemeeter::UI::D2D::Trackers::Dirty& dirtyTracker,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Environment::ITimer& timer,
	::Voicemeeter::UI::D2D::Graphics::Canvas& canvas,
	TStrip& strip,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	if (animations) {
		using Glyph = ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Animations::Knob<
			::Voicemeeter::UI::D2D::Graphics::Glyphs::Knob>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ ToLabel(strip.get_Id()), canvas, dirtyTracker } };
		return ComposeKnob(
			direction, marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription);
	} else {
		using Glyph = ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Knob<
			::Voicemeeter::UI::D2D::Graphics::Glyphs::Knob>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ ToLabel(strip.get_Id()), canvas, dirtyTracker } };
		return ComposeKnob(
			direction, marginDirection,
			first,
			focusTracker, inputTracker,
			timer, pGlyph,
			strip, mixer, subscription);
	}
}

template<::Voicemeeter::UI::Direction MarginDirection, typename TGlyph, typename TInput, typename TOutput>
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
		::Voicemeeter::UI::D2D::Decorators::Interactivity::Plug<TGlyph, decltype(changeNotify)>>;
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
template<typename TGlyph, typename TInput, typename TOutput>
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
		return ComposePlug<::Voicemeeter::UI::Direction::Right>(
			first,
			focusTracker, inputTracker,
			pGlyph,
			input, output, mixer, subscription
		);
	case ::Voicemeeter::UI::Direction::Down:
		return ComposePlug<::Voicemeeter::UI::Direction::Down>(
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
	::Voicemeeter::UI::D2D::Trackers::Dirty& dirtyTracker,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Voicemeeter::UI::D2D::Graphics::Canvas& canvas,
	TInput& input, TOutput& output,
	::Voicemeeter::Adapters::Multiclient::Cherry& mixer,
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription
) {
	if (animations) {
		using Glyph = ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Animations::Plug<
			::Voicemeeter::UI::D2D::Graphics::Glyphs::Plug>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ ToLabel(output.get_Id()), canvas, dirtyTracker } };
		return ComposePlug(
			marginDirection,
			first,
			focusTracker, inputTracker,
			pGlyph,
			input, output, mixer, subscription
		);
	} else {
		using Glyph = ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Plug<
			::Voicemeeter::UI::D2D::Graphics::Glyphs::Plug>;
		::std::unique_ptr<Glyph> pGlyph{ new Glyph{ ToLabel(output.get_Id()), canvas, dirtyTracker } };
		return ComposePlug(
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
	::Voicemeeter::UI::D2D::Trackers::Dirty& dirtyTracker,
	::Voicemeeter::UI::Trackers::Focus& focusTracker,
	::Voicemeeter::UI::Trackers::Input& inputTracker,
	::Voicemeeter::UI::D2D::Graphics::Canvas& canvas
) {
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription = m_mixer.get_Subscription<Cherry>();
	::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpComponent{};
	if (m_network) {
		cpComponent.push_back(::std::move(
			ComposeVban(
				m_animations,
				dirtyTracker, focusTracker, inputTracker,
				canvas,
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
				dirtyTracker, focusTracker, inputTracker,
				m_compositionTimer, canvas,
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
					dirtyTracker, focusTracker, inputTracker,
					canvas,
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
					dirtyTracker, focusTracker, inputTracker,
					canvas,
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
				dirtyTracker, focusTracker, inputTracker,
				m_compositionTimer, canvas,
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
					dirtyTracker, focusTracker, inputTracker,
					canvas,
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
					dirtyTracker, focusTracker, inputTracker,
					canvas,
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
				dirtyTracker, focusTracker, inputTracker,
				m_compositionTimer, canvas,
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
				dirtyTracker, focusTracker, inputTracker,
				m_compositionTimer, canvas,
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