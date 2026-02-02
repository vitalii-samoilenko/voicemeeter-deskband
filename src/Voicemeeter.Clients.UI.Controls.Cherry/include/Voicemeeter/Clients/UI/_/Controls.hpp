#ifndef VOICEMEETER_CLIENTS_UI___CONTROLS_HPP
#define VOICEMEETER_CLIENTS_UI___CONTROLS_HPP

#include <array>
#include <bitset>
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <utility>

#include "wheel.hpp"

#include "Voicemeeter/Cherry.hpp"
#include "Voicemeeter/UI/Adapters/Glyph.hpp"
#include "Voicemeeter/UI/Adapters/Graphics/Animated.hpp"
#include "Voicemeeter/UI/Adapters/Interactivity/Controllers/Circular.hpp"
#include "Voicemeeter/UI/Adapters/Interactivity/Controllers/Plug.hpp"
#include "Voicemeeter/UI/Adapters/Interactivity/Controllers/StripKnob.hpp"
#include "Voicemeeter/UI/Adapters/Interactivity/Transparent.hpp"
#include "Voicemeeter/UI/Adapters/State.hpp"
#include "Voicemeeter/UI/Decorators/HitTest.hpp"
#include "Voicemeeter/UI/Decorators/Interactivity/Carousel.hpp"
#include "Voicemeeter/UI/Decorators/Interactivity/Grab.hpp"
#include "Voicemeeter/UI/Decorators/Interactivity/Point.hpp"
#include "Voicemeeter/UI/Decorators/Interactivity/StripKnob.hpp"
#include "Voicemeeter/UI/Decorators/Padding.hpp"
#include "Voicemeeter/UI/Graphics/Bundles/Knob.hpp"
#include "Voicemeeter/UI/Graphics/Bundles/Plug.hpp"
#include "Voicemeeter/UI/Graphics/Bundles/Vban.hpp"
#include "Voicemeeter/UI/Graphics/Theme.hpp"
#include "Voicemeeter/UI/Panels/Stack.hpp"
#include "Voicemeeter/UI/Policies/Control/Updates/Animated/Plug.hpp"
#include "Voicemeeter/UI/Policies/Control/Updates/Animated/StripKnob.hpp"
#include "Voicemeeter/UI/Policies/Control/Updates/Animated/Vban.hpp"
#include "Voicemeeter/UI/Policies/Control/Updates/Plug.hpp"
#include "Voicemeeter/UI/Policies/Control/Updates/StripKnob.hpp"
#include "Voicemeeter/UI/Policies/Control/Updates/Vban.hpp"
#include "Voicemeeter/UI/Policies/Orientation/Directions/Axis.hpp"
#include "Voicemeeter/UI/Policies/Size/Scales/PreserveRatio.hpp"
#include "Voicemeeter/UI/States/Plug.hpp"
#include "Voicemeeter/UI/States/StripKnob.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace _ {
				namespace Controls {
					enum flag : size_t {
						animated = 0,
						vban = 1,
						offset = 1
					};

					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					class Composition;

					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					::std::unique_ptr<
						Composition<
							TTimer,
							TMixer,
							TToolkit,
							TFocusTracker>
					> Subscribe(
						TTimer &timer,
						TMixer &mixer,
						TToolkit &toolkit,
						TFocusTracker &focusTracker,
						::std::bitset<
							flag::offset
							+ TMixer::InputSize
							+ TMixer::OutputSize
						> const &enabled);

					template<typename TMixer>
					class bag;

					template<typename TMixer>
					void PickAndUpdateStrip(TMixer &mixer, UI::States::StripKnob const &state);
					template<typename TMixer>
					void PickAndUpdatePlug(TMixer &mixer, UI::States::Plug const &state);
					template<
						typename TMixer,
						typename TMixer::Strips From>
					void PickAndUpdatePlug(TMixer &mixer, UI::States::Plug const &state);

					template<
						typename TMixer,
						typename TStripKnob>
					void PickAndSubscribeStripKnob(size_t target,
						TStripKnob &stripKnob, bag<TMixer> &tokens);
					template<
						typename TMixer,
						typename TPlug>
					void PickAndSubscribePlug(size_t from, size_t to,
						TPlug &plug, bag<TMixer> &tokens);
					template<
						typename TMixer,
						typename TMixer::Strips From,
						typename TPlug>
					void PickAndSubscribePlug(size_t to,
						TPlug &plug, bag<TMixer> &tokens);

					namespace Policies {
						namespace Control {
							namespace Notifications {
								template<typename TMixer>
								class Vban final {
								public:
									using state_t = num_t;

									inline explicit VbanNotify(TMixer &mixer)
										: _mixer{ mixer } {

									};
									VbanNotify() = delete;
									VbanNotify(VbanNotify const &) = delete;
									inline VbanNotify(VbanNotify &&) = default;

									inline ~VbanNotify() = default;

									VbanNotify & operator=(VbanNotify const &) = delete;
									VbanNotify & operator=(VbanNotify &&) = delete;

									inline void operator()(state_t state) const {
										_mixer.set_Vban<bag<TMixer>>(
											static_cast<bool>(state));
									};

								private:
									TMixer &_mixer;
								};

								template<typename TMixer>
								class StripKnob final {
								public:
									using state_t = UI::States::StripKnob;

									inline explicit StripKnobNotify(TMixer &mixer)
										: _mixer{ mixer } {

									};
									StripKnobNotify() = delete;
									StripKnobNotify(StripKnobNotify const &) = delete;
									inline StripKnobNotify(StripKnobNotify &&) = default;

									inline ~StripKnobNotify() = default;

									StripKnobNotify & operator=(StripKnobNotify const &) = delete;
									StripKnobNotify & operator=(StripKnobNotify &&) = delete;

									inline void operator()(state_t const &state) const {
										PickAndUpdateStrip<TMixer>(
											mixer, state);
									};

								private:
									TMixer &_mixer;
								};

								template<typename TMixer>
								class Plug final {
								public:
									using state_t = UI::States::Plug;

									inline explicit PlugNotify(TMixer &mixer)
										: _mixer{ mixer } {

									};
									PlugNotify() = delete;
									PlugNotify(PlugNotify const &) = delete;
									inline PlugNotify(PlugNotify &&) = default;

									inline ~PlugNotify() = default;

									PlugNotify & operator=(PlugNotify const &) = delete;
									PlugNotify & operator=(PlugNotify &&) = delete;

									inline void operator()(state_t const &state) const {
										PickAndUpdatePlug<TMixer>(
											mixer, state);
									};

								private:
									TMixer &_mixer;
								};
							}
							namespace Updates {
								template<
									typename TAnimated,
									typename TInstant>
								class ToggleAnimation final {
								public:
									using context_t = typename TAnimated::context_t;
									using state_t = typename TAnimated::state_t;

									inline ToggleAnimation(
										bool on,
										TAnimated &&animated = TAnimated{},
										TInstant &&instant = TInstant{})
										: _on{ on }
										, _animated{ ::std::move(animated) }
										, _instant{ ::std::move(instant) }

									};
									ToggleAnimation() = delete;
									ToggleAnimation(ToggleAnimation const &) = delete;
									inline ToggleAnimation(ToggleAnimation &&) = default;

									inline ~ToggleAnimation() = default;

									ToggleAnimation & operator=(ToggleAnimation const &) = delete;
									ToggleAnimation & operator=(ToggleAnimation &&) = delete;

									template<typename TControl>
									inline void operator()(TControl &control, state_t const &state) const {
										if (_on) {
											_animated(control, state);
										} else {
											_instant(control, state);
										}
									};

								private:
									bool _on;
									TAnimated _animated;
									TInstant _instant;
								};
							}
						}
					}
					namespace Decorators {
						template<typename TControl>
						class ToggleVisibility : public TControl {
						public:
							template<typename... Args>
							inline ToggleVisibility(
								bool on,
								Args &&...args)
								: TControl{ ::std::forward<Args>(args) ... }
								, _on{ on } {

							};
							ToggleVisibility() = delete;
							ToggleVisibility(ToggleVisibility const &) = delete;
							ToggleVisibility(ToggleVisibility &&) = delete;

							inline ~ToggleVisibility() = default;

							ToggleVisibility & operator=(ToggleVisibility const &) = delete;
							ToggleVisibility & operator=(ToggleVisibility &&) = delete;

							inline void Move(vector_t const &point) {
								if (_on) {
									TControl::Move(point);
								}
							};
							inline void Rescale(vecot_t const &vertex) {
								if (_on) {
									TControl::Rescale(vertex);
								}
							};

						private:
							bool _on;
						};
					}

					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					using Vban = UI::Decorators::HitTest<
						UI::Decorators::Interactivity::Point<
							UI::Decorators::Interactivity::Carousel<
							UI::Adapters::Interactivity::Controllers::Circular<
							UI::Adapters::State<
								UI::Adapters::Interactivity::Transparent<
								UI::Adapters::Glyph<
									UI::Adapters::Graphics::Animated<
										UI::Adapters::Graphics::Context<
											UI::Graphics::Bundles::Vban,
											UI::Policies::Control::Updates::Animated::VbanContext<TToolkit>>,
										TToolkit,
										UI::Policies::Control::Updates::Animated::VbanFrame<TToolkit>,
										slice_t>,
									UI::Policies::Size::Scales::PreserveRatio
								>>,
								num_t,
								Policies::Control::Notifications::Vban<TMixer>,
								Policies::Control::Updates::ToggleAnimation<
									UI::Policies::Control::Updates::Animated::Vban<TToolkit>,
									UI::Policies::Control::Updates::Vban<TToolkit>>
							>>>,
							TFocusTracker
						>>;
						template<
							typename TTimer,
							typename TMixer,
							typename TToolkit,
							typename TFocusTracker>
						using StripKnob = UI::Decorators::HitTest<
							UI::Decorators::Interactivity::Grab<
								UI::Decorators::Interactivity::StripKnob<
									UI::Adapters::Interactivity::Controllers::StripKnob<
									UI::Adapters::State<
										UI::Adapters::Interactivity::Transparent<
										UI::Adapters::Glyph<
											UI::Adapters::Graphics::Animated<
												UI::Adapters::Graphics::Context<
													UI::Graphics::Bundles::Knob,
													UI::Policies::Control::Updates::Animated::StripKnobContext<TToolkit>>,
												TToolkit,
												UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>,
												slice_t,
												slice_t>,
											UI::Policies::Size::Scales::PreserveRatio
										>>,
										UI::States::StripKnob,
										Policies::Control::Notifications::StripKnob<TMixer>,
										Policies::Control::Updates::ToggleAnimation<
											UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
											UI::Policies::Control::Updates::StripKnob<TToolkit>>
									>>,
									TTimer,
									UI::Policies::Orientation::Directions::Axis>,
								TFocusTracker
							>>;
					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					using Plug = UI::Decorators::HitTest<
						UI::Decorators::Interactivity::Point<
							UI::Decorators::Interactivity::Carousel<
							UI::Adapters::Interactivity::Controllers::Plug<
							UI::Adapters::State<
								UI::Adapters::Interactivity::Transparent<
								UI::Adapters::Glyph<
									UI::Adapters::Graphics::Animated<
										UI::Adapters::Graphics::Context<
											UI::Graphics::Bundles::Plug,
											UI::Policies::Control::Updates::Animated::PlugContext<TToolkit>>,
										TToolkit,
										UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>,
										slice_t>,
									UI::Policies::Size::Scales::PreserveRatio
								>>,
								UI::States::Plug,
								Policies::Control::Notifications::Plug<TMixer>,
								Policies::Control::Updates::ToggleAnimation<
									UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
									UI::Policies::Control::Updates::Plug<TToolkit>>
							>>>,
							TFocusTracker
						>>;

					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryOutputSection = UI::Panels::Stack<
						UI::Policies::Orientation::Directions::Axis,
						UI::Policies::Size::Scales::PreserveRatio,
						StripKnob<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
						>>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
						>>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
					>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryMixerBlock = UI::Panels::Stack<
						UI::Policies::Orientation::Directions::Axis,
						UI::Policies::Size::Scales::PreserveRatio,
						Plug<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							Plug<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
					>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryInputBlock = UI::Panels::Stack<
						UI::Policies::Orientation::Directions::Axis,
						UI::Policies::Size::Scales::PreserveRatio,
						StripKnob<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							CherryMixerBlock<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
						>>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							CherryMixerBlock<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
					>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryInputSection = UI::Panels::Stack<
						UI::Policies::Orientation::Directions::Axis,
						UI::Policies::Size::Scales::PreserveRatio,
						CherryInputBlock<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						UI::Decorators::Padding<
							CherryInputBlock<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							UI::Policies::Size::Scales::PreserveRatio
					>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryFrontPanel = UI::Decorators::Padding<
						UI::Panels::Stack<
							UI::Policies::Orientation::Directions::Axis,
							UI::Policies::Size::Scales::PreserveRatio,
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								Vban<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>,
							Decorators::ToggleVisibility<
							UI::Decoratos::Padding<
								CherryInputSection<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>,
							Decorators::ToggleVisibility<
							UI::Decoratos::Padding<
								CherryOutputSection<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>>,
						UI::Policies::Size::Scales::PreserveRatio>;

					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					class Composition<
						TTimer,
						Cherry,
						TToolkit,
						TFocusTracker
					> : public CherryFrontPanel<
						TTimer,
						TToolkit,
						TFocusTracker
					> {
					public:
						template<typename... Args>
						inline CherryComposition(
							bag<TMixer> &&tokens,
							Args &&...args)
							: TPanel{ ::std::forward<Args>(args) ... }
							, _tokens{ ::std::move(tokens) } {

						};
						CherryComposition() = delete;
						CherryComposition(CherryComposition const &) = delete;
						CherryComposition(CherryComposition &&) = delete;

						inline ~CherryComposition() = default;

						CherryComposition & operator=(CherryComposition const &) = delete;
						CherryComposition & operator=(CherryComposition &&) = delete;

					private:
						bag<TMixer> _tokens;
					};

					template<typename... TTokens>
					class caggregator {
					public:
						inline explicit caggregator(TTokens &&...tokens)
							: _tokens{ ::std::move(tokens) ... }
							, _callback{}
							, _levels(0, sizeof...(TTokens)) {

						};
						caggregator() = delete;
						caggregator(caggregator const &) = delete;
						inline caggregator(caggregator &&) = default;

						inline ~caggregator() = default;

						caggregator & operator=(caggregator const &) = delete;
						caggregator & operator=(caggregator &&) = delete;

						template<typename Fn>
						void on_MaxLevel(Fn &&callback) {
							_callback = ::std::forward<Fn>(callback);
							size_t temp{ 0 };
							(_tokens.on_Level([
								&levels = _levels,
								&callback = _callback,
								i = temp++
							](num_t value)->void {
								num_t before{ max(levels) };
								levels[i] = value;
								num_t after{ max(levels) };
								if (before == after) {
									return;
								}
								_callback(after);
							})
							, ...);
						};

					private:
						::std::tuple<TTokens ...> _tokens;
						::std::function<void(num_t)> _callback;
						vector_t _levels;
					};
					template<>
					class bag<Cherry> final {
					public:
						inline explicit bag(Cherry &mixer)
							: _mixer{ mixer.Subscribe<bag>() }
							, _strips{
								mixer.get_Strip<Cherry::Strips::P>()
									.Subscribe<bag>(),
								mixer.get_Strip<Cherry::Strips::V>()
									.Subscribe<bag>(),
								mixer.get_Strip<Cherry::Strips::A1>()
									.Subscribe<bag>(),
								mixer.get_Strip<Cherry::Strips::A2>()
									.Subscribe<bag>(),
								mixer.get_Strip<Cherry::Strips::B1>()
									.Subscribe<bag>(),
								mixer.get_Strip<Cherry::Strips::B2>()
									.Subscribe<bag>()
							}
							, _channels{
								caggregator2{
									mixer.get_Strip<Cherry::Strips::P>()
										.get_Channel<0>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::P>()
										.get_Channel<1>()
										.Subscribe<bag>()
								},
								caggregator8{
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<0>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<1>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<2>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<3>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<4>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<5>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<6>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::V>()
										.get_Channel<7>()
										.Subscribe<bag>()
								},
								caggregator8{
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<0>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<1>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<2>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<3>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<4>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<5>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<6>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A1>()
										.get_Channel<7>()
										.Subscribe<bag>()
								},
								caggregator8{
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<0>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<1>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<2>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<3>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<4>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<5>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<6>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::A2>()
										.get_Channel<7>()
										.Subscribe<bag>()
								},
								caggregator8{
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<0>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<1>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<2>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<3>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<4>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<5>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<6>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B1>()
										.get_Channel<7>()
										.Subscribe<bag>()
								},
								caggregator8{
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<0>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<1>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<2>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<3>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<4>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<5>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<6>()
										.Subscribe<bag>(),
									mixer.get_Strip<Cherry::Strips::B2>()
										.get_Channel<7>()
										.Subscribe<bag>()
								}
							} {

						};
						bag() = delete;
						bag(bag const &) = delete;
						inline bag(bag &&) = default;

						inline ~bag() = default;

						bag & operator=(bag const &) = delete;
						bag & operator=(bag &&) = delete;

					auto & get_Mixer() {
						return _mixer;
					};
					template<Cherry::Strips Target>
					auto & get_Strip() {
						return ::std::get<Target>(_strips);
					};
					template<Cherry::Strips Target:
					auto & get_Channels() {
						return ::std::get<Target>(_channels);
					};

					private:
						using caggregator2 = caggregator<
							Cherry::Channel::token,
							Cherry::Channel::token
						>;
						using caggregator8 = caggregator<
							Cherry::Channel::token,
							Cherry::Channel::token,
							Cherry::Channel::token,
							Cherry::Channel::token,
							Cherry::Channel::token,
							Cherry::Channel::token,
							Cherry::Channel::token,
							Cherry::Channel::token
						>;

						Cherry::token _mixer;
						::std::tuple<
							Cherry::PIStrip::token,
							Cherry::VIStrip::token,
							Cherry::POStrip::token,;
							Cherry::POStrip::token,
							Cherry::VOStrip::token,
							Cherry::VOStrip::token
						> _strips;
						::std::tuple<
							caggregator2,
							caggregator8,
							caggregator8,
							caggregator8,
							caggregator8,
							caggregator8,
						> _channels;
					};

					template<
						typename TMixer,
						typename TMixer::Strips Target>
					inline void UpdateStrip(TMixer &mixer, UI::States::StripKnob const &state) {
						mixer.get_Strip<Target>()
							.set_Mute<bag<TMixer>>(state.toggle);
						mixer.get_Strip<Target>()
							.set_Gain<bag<TMixer>>(state.degree);
					};
					template<>
					inline void PickAndUpdateStrip<Cherry>(Cherry &mixer, UI::States::StripKnob const &state) {
						switch (state.target) {
						case Cherry::Strips::P:
							UpdateStrip<Cherry,
								Cherry::Strips::P>(
								mixer, state);
							break;
						case Cherry::Strips::V:
							UpdateStrip<Cherry,
								Cherry::Strips::V>(
								mixer, state);
							break;
						case Cherry::Strips::A1:
							UpdateStrip<Cherry,
								Cherry::Strips::A1>(
								mixer, state);
							break;
						case Cgerry::Strips::A2:
							UpdateStrip<Cherry,
								Cherry::Strips::A2>(
								mixer, state);
							break;
						case Cherry::Strips::B1:
							UpdateStrip<Cherry,
								Cherry::Strips::B1>(
								mixer, state);
							break;
						case Cherry::Strips::B2:
							UpdateStrip<Cherry,
								Cherry::Strips::B2>(
								mixer, state);
							break;
						}
					};
					template<
						typename TMixer,
						typename TMixer::Strips From,
						typename TMixer::Strips To>
					inline void UpdatePlug(TMixer &mixer, UI::States::Plug const &state) {
						mixer.set_Plug<From, To, bag<TMixer>>(state.toggle);
					};
					template<Cherry::Strips From>
					inline void PickAndUpdatePlug<Cherry>(Cherry &mixer, UI::States::Plug const &state) {
						switch (state.to) {
						case Cherry::Strips::A1:
							UpdatePlug<Cherry,
								From, Cherry::Strips::A1>(
								mixer, state);
							break;
						case Cherry::Strips::A2:
							UpdatePlug<Cherry,
								From, Cherry::Strips::A2>(
								mixer, state);
							break;
						case Cherry::Strips::B1:
							UpdatePlug<Cherry,
								From, Cherry::Strips::B1>(
								mixer, state);
							break;
						case Cherry::Strips::B2:
							UpdatePlug<Cherry,
								From, Cherry::Strips::B2>(
								mixer, state);
							break;
						}
					};
					template<>
					inline void PickAndUpdatePlug<Cherry>(Cherry &mixer, UI::States::Plug const &state) {
						switch (state.from) {
						case Cherry::Strips::P:
							PickAndUpdatePlug<Cherry,
								Cherry::Strips::P>(
								mixer, state);
							break;
						case Cherry::Strips::V:
							PickAndUpdatePlug<Cherry,
								Cherry::Strips::V>(
								mixer, state);
							break;
						}
					};

					template<
						typename TMixer,
						typename TMixer::Strips Target,
						typename TStripKnob>
					inline void SubscribeStripKnob(TStripKnob &stripKnob, bag<TMixer> &tokens) {
						using state_t = typename TStripKnob::state_t;
						state_t state{ stripKnob.get_State() };
						state.target = Target;
						stripKnob.set_State(state);
						tokens.get_Strip<Target>()
							.on_Gain([
								&stripKnob
							](num_t value)->void {
								state_t state{ stripKnob.get_State() };
								state.degree = value;
								stripKnob.set_State(state);
							});
						tokens.get_Strip<Target>()
							.on_Mute([
								&stripKnob
							](bool value)->void {
								state_t state{ stripKnob.get_State() };
								state.toggle = value;
								stripKnob.set_State(state);
							});
						tokens.get_Channels<Target>()
							..on_MaxLevel([
								&stripKnob
							](num_t value)->void {
								state_t state{ stripKnob.get_State() };
								state.level = value;
								stripKnob.set_State(state);
							});
					};
					template<typename TStripKnob>
					inline void PickAndSubscribeStripKnob<Cherry, TStripKnob>(size_t target,
						TStripKnob &stripKnob, bag<Cherry> &tokens) {
						switch (target) {
						case Cherry::Strips::P:
							SubscribeStripKnob<Cherry,
								Cherry::Strips::P>(
								stripKnob, tokens);
							break;
						case Cherry::Strips::V:
							SubscribeStripKnob<Cherry,
								Cherry::Strips::V>(
								stripKnob, tokens);
							break;
						case Cherry::Strips::A1:
							SubscribeStripKnob<Cherry,
								Cherry::Strips::A1>(
								stripKnob, tokens);
							break;
						case Cherry::Strips::A2:
							SubscribeStripKnob<Cherry,
								Cherry::Strips::A2>(
								stripKnob, tokens);
							break;
						case Cherry::Strips::B1:
							SubscribeStripKnob<Cherry,
								Cherry::Strips::B1>(
								stripKnob, tokens);
							break;
						case Cherry::Strips::B2:
							SubscribeStripKnob<Cherry,
								Cherry::Strips::B2>(
								stripKnob, tokens);
							break;
						}
					};
					template<
						typename TMixer,
						typename TMixer::Strips From, typename TMixer::Strips To,
						typename TPlug>
					inline void SubscribePlug(TPlug &plug, bag<TMixer> &tokens) {
						using state_t = typename TPlug::state_t;
						state_t state{ plug.get_State() };
						state.from = From;
						state.to = To;
						plug.set_State(state);
						tokens.get_Mixer()
							.on_Plug<From, To>([
								&plug
							](bool value)->void {
								state_t state{ plug.get_State() };
								state.toggle = value;
								plug.set_State(state);
							});
					};
					template<
						typename TMixer::Strips From,
						typename TPlug>
					inline void PickAndSubscribePlug<Cherry, From, TPlug>(size_t to,
						TPlug &plug, bag<Cherry> &tokens) {
						switch (to) {
						case Cherry::Strips::A1:
							SubscribePlug<Cherry,
								From, Cherry::Strips::A1>(
								plug, tokens);
							break;
						case Cherry::Strips::A2:
							SubscribePlug<Cherry,
								From, Cherry::Strips::A2>(
								plug, tokens);
							break;
						case Cherry::Strips::B1:
							SubscribePlug<Cherry,
								From, Cherry::Strips::B1>(
								plug, tokens);
							break;
						case Cherry::Strips::B2:
							SubscribePlug<Cherry,
								From, Cherry::Strips::B2>(
								plug, tokens);
							break;
						}
					};
					template<typename TPlug>
					inline void PickAndSubscribePlug<Cherry, TPlug>(size_t from, size_t to,
						TPlug &plug, bag<Cherry> &tokens) {
						switch (from) {
						case Cherry::Strips::P:
							PickAndSubscribePlug<Cherry,
								Cherry::Strips::P>(to,
								plug, tokens);
							break;
						case Cherry::Strips::V:
							PickAndSubscribePlug<Cherry,
								Cherry::Strips::V>(to,
								plug, tokens);
							break;
						}
					};

					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					::std::unique_ptr<
						Composition<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>
					> Subscribe<
						TTimer,
						Cherry,
						TToolkit,
						TFocusTracker
					>(
						TTimer &time,
						Cherry &mixer,
						TToolkit &toolkit,
						TFocusTracker &focusTracker,
						::std::bitset<
							flag::offset
							+ Cherry::InputSize
							+ Cherry::OutputSize
						> const &enabled) {
						constexpr num_t PanelH{ push(32) };
						constexpr num_t PanelPaddingL{ push(1) };
						constexpr num_t PanelPaddingT{ 0 };
						constexpr num_t PanelPaddingR{ 0 };
						constexpr num_t PanelPaddingB{ 0 };
						constexpr num_t VbanH{ PanelH };
						constexpr num_t VbanW{ push(64) };
						constexpr num_t KnobW{ PanelH };
						constexpr num_t KnobH{ PanelH };
						constexpr num_t PlugH{ push(12) };
						constexpr num_t PlugW{ PanelH };
						constexpr num_t PlugPaddingL{ 0 };
						constexpr num_t PlugPaddingT{ push(2) };
						constexpr num_t PlugPaddingR{ 0 };
						constexpr num_t PlugPaddingB{ 0 };
						constexpr num_t MixerBlockH{ PlugH + PlugPaddingT + PlugH };
						constexpr num_t MixerBlockW{ PlugW };
						constexpr num_t InputBlockH{ PanelH };
						constexpr num_t InputBlockW{ KnobW + 2 * (PanelPaddingL + MixerBlockW) };
						constexpr num_t InputSectionH{ PanelH };
						constexpr num_t InputSectionW{ InputBlockW + PanelPaddingL + InputBlockW };
						constexpr num_t OutputSectionH{ PanelH };
						constexpr num_t OutputSectionW{ KnobW + 3 * (PanelPaddingL + KnobW) };
						constexpr num_t PanelW{
							PanelPaddingL + VbanW
							+ PanelPaddingL + InputSectionW
							+ PanelPaddingL + OutputSectionW
							+ PanelPaddingL
						};
						num_t enabledInputs{
							static_cast<num_t>(
								enabled.test(flag::offset + Cherry::Strips::P))
							+ enabled.test(flag::offset + Cherry::Strips::V)
						};
						num_t enabledOutputs{
							static_cast<num_t>(
								enabled.test(flag::offset + Cherry::Strips::A1))
							+ enabled.test(flag::offset + Cherry::Strips::A2)
							+ enabled.test(flag::offset + Cherry::Strips::B1)
							+ enabled.test(flag::offset + Cherry::Strips::B2)
						};
						auto vban = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								Vban<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							enabled.test(flag::vban),
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							0, 1, 1,
							Policies::Control::Notifications::Vban<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Vban<TToolkit>,
								UI::Policies::Control::Updates::Vban<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Vban<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Vban<TToolkit>{ toolkit }
							},
							vector_t{ VbanW, VbanH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::VbanFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1 = ::std::make_unique<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							timer,
							UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{ KnobW, KnobH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2 = ::std::make_unique<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							timer,
							UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{ KnobW, KnobH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o1 = ::std::make_unique<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							timer,
							UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{ KnobW, KnobH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								StripKnob<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							timer,
							UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{ KnobW, KnobH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o3 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								StripKnob<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							2 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							timer,
							UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{ KnobW, KnobH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								StripKnob<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							3 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							timer,
							UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{ KnobW, KnobH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1o1 = ::std::make_unique<
							Plug<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledOutputs,
							vector_t{ PlugPaddingL, PlugPaddingT },
							vector_t{ PlugPaddingR, PlugPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1o3 = ::std::make_unique<
							Plug<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							3 < enabledOutputs,
							vector_t{ PlugPaddingL, PlugPaddingT },
							vector_t{ PlugPaddingR, PlugPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2o1 = ::std::make_unique<
							Plug<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledOutputs,
							vector_t{ PlugPaddingL, PlugPaddingT },
							vector_t{ PlugPaddingR, PlugPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2o3 = ::std::make_unique<
							Plug<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							3 < enabledOutputs,
							vector_t{ PlugPaddingL, PlugPaddingT },
							vector_t{ PlugPaddingR, PlugPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flag::animated),
								UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{ PlugW, PlugH },
							UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						if (enabled.test(flag::vban)) {
							vban.set_FrameColor(toolkit.get_Theme()
								.Inactive);
						}
						if (1 < enabledInputs) {
							i2.set_FrameColor(toolkit.get_Theme()
								.Inactive);
							i2.set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
							if (3 < enabledOutputs) {
								i2o4.set_FrameColor(toolkit.get_Theme()
									.Inactive);
							}
							if (2 < enabledOutputs) {
								i2o3.set_FrameColor(toolkit.get_Theme()
									.Inactive);
							}
							if (1 < enabledoutputs) {
								i2o2.set_framecolor(toolkit.get_theme()
									.inactive);
							}
							if (0 < enabledoutputs) {
								i2o1.set_framecolor(toolkit.get_theme()
									.inactive);
							}
						}
						if (0 < enabledInputs) {
							i1.set_FrameColor(toolkit.get_Theme()
								.Inactive);
							i1.set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
							if (3 < enabledOutputs) {
								i1o4.set_FrameColor(toolkit.get_Theme()
									.Inactive);
							}
							if (2 < enabledOutputs) {
								i1o3.set_FrameColor(toolkit.get_Theme()
									.Inactive);
							}
							if (1 < enabledoutputs) {
								i1o2.set_framecolor(toolkit.get_theme()
									.inactive);
							}
							if (0 < enabledoutputs) {
								i1o1.set_framecolor(toolkit.get_theme()
									.inactive);
							}
						}
						if (3 < enabledOutputs) {
							o4.set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o4.set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						if (2 < enabledOutputs) {
							o3.set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o3.set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						if (1 < enabledoutputs) {
							o2.set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o2.set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						if (0 < enabledoutputs) {
							o1.set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o1.set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						::std::array<size_t, 6>targets(
							::std::numeric_limits<size_t>::max());
						{
							auto current = targets.begin();
							if (enabled.test(flag::offset + Cherry::Strips::P)) {
								*(current++) = Cherry::Strips::P;
							}
							if (enabled.test(flag::offset + Cherry::Strips::V)) {
								*(current++) = Cherry::Strips::V;
							}
						}
						{
							auto current = targets.begin() + 2;
							if (enabled.test(flag::offset + Cherry::Strips::A1)) {
								*(current++) = Cherry::Strips::A1;
							}
							if (enabled.test(flag::offset + Cherry::Strips::A2)) {
								*(current++) = Cherry::Strips::A2;
							}
							if (enabled.test(flag::offset + Cherry::Strips::B1)) {
								*(current++) = Cherry::Strips::B1;
							}
							if (enabled.test(flag::offset + Cherry::Strips::B2)) {
								*(current++) = Cherry::Strips::B2;
							}
						}
						bag<Cherry> tokens{ mixer };
						if (enabled.test(flag::vban)) {
							tokens.get_Mixer()
								.on_Vban([
									&control = &vban
								](bool value)->void {
									control.set_State(
										static_cast<num_t>(value));
								});
						}
						PickAndSubscribeStripKnob<Cherry>(targets[0],
							*i1, tokens);
						PickAndSubscribeStripKnob<Cherry>(targets[1],
							*i2, tokens);
						PickAndSubscribeStripKnob<Cherry>(targets[2],
							*o1, tokens);
						PickAndSubscribeStripKnob<Cherry>(targets[3],
							*o2, tokens);
						PickAndSubscribeStripKnob<Cherry>(targets[4],
							*o3, tokens);
						PickAndSubscribeStripKnob<Cherry>(targets[5],
							*o4, tokens);
						PickAndSubscribePlug<Cherry>(targets[0], targets[2],
							*i1o1, tokens);
						PickAndSubscribePlug<Cherry>(targets[0], targets[3],
							*i1o2, tokens);
						PickAndSubscribePlug<Cherry>(targets[0], targets[4],
							*i1o3, tokens);
						PickAndSubscribePlug<Cherry>(targets[0], targets[5],
							*i1o4, tokens);
						PickAndSubscribePlug<Cherry>(targets[1], targets[2],
							*i2o1, tokens);
						PickAndSubscribePlug<Cherry>(targets[1], targets[3],
							*i2o2, tokens);
						PickAndSubscribePlug<Cherry>(targets[1], targets[4],
							*i2o3, tokens);
						PickAndSubscribePlug<Cherry>(targets[1], targets[5],
							*i2o4, tokens);
						auto i1o1o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ MixerBlockW, MixerBlockH },
							UI::Policies::Orientation::Directions::Axis{ 1 }.
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1o1),
							::std::move(i1o2));
						};
						auto i1o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							2 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ MixerBlockW, MixerBlockH },
							UI::Policies::Orientation::Directions::Axis{ 1 }.
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1o3),
							::std::move(i1o4));
						auto i1o1o2o3o4 = ::std::make_unique<
							CherryInputBlock<
								TTimer,
								TToolkit,
								TFocusTracker>
						>(
							vector_t{ InputBlockW, InputBlockH },
							UI::Policies::Orientation::Directions::Axis{ 0 },
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1),
							::std::move(i1o1o2),
							::std::move(i1o3o4));
						auto i2o1o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ MixerBlockW, MixerBlockH },
							UI::Policies::Orientation::Directions::Axis{ 1 }.
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i2o1),
							::std::move(i2o2));
						};
						auto i2o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							2 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ MixerBlockW, MixerBlockH },
							UI::Policies::Orientation::Directions::Axis{ 1 }.
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i2o3),
							::std::move(i2o4));
						auto i2o1o2o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryInputBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledInputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ InputBlockW, InputBlockH },
							UI::Policies::Orientation::Directions::Axis{ 0 },
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i2),
							::std::move(i2o1o2),
							::std::move(i2o3o4));
						auto i1i2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryInputSection<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledInputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ InputSectionW, InputSectionH },
							UI::Policies::Orientation::Directions::Axis{ 0 },
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1o1o2o3o4),
							::std::move(i2o1o2o3o4));
						auto o1o2o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							UI::Decorators::Padding<
								CherryOutputSection<
									TTimer,
									TToolkit,
									TFocusTracker>,
								UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledOutputs,
							vector_t{ PanelPaddingL, PanelPaddingT },
							vector_t{ PanelPaddingR, PanelPaddingB },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ OutputSectionW, OutputSectionH },
							UI::Policies::Orientation::Directions::Axis{ 0 },
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(o1),
							::std::move(o2),
							::std::move(o3),
							::std::move(o4));
						return ::std::make_unique<
							Composition<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>
						>(
							::std::move(tokens),
							vector_t{ PanelPaddingR, PanelPaddingB },
							vector_t{ PanelPaddingL, PanelPaddingT },
							UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{ PanelW, PanelH },
							UI::Policies::Orientation::Directions::Axis{ 0 },
							UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(vban),
							::std::move(i1i2),
							::std::move(o1o2o3o4));
					};
				}
			}
		}
	}
}

#endif
