#ifndef VOICEMEETER_CLIENTS_UI___CONTROLS_HPP
#define VOICEMEETER_CLIENTS_UI___CONTROLS_HPP

#include <array>
#include <bitset>
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <type_traits>
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
	namespace UI {
		namespace Layouts {
			namespace Cherry {
				namespace Panel {
					constexpr num_t Height{ push(40) };
					namespace Advance {
						constexpr num_t Left{ push(1) };
						constexpr num_t Top{ 0 };
						constexpr num_t Right{ 0 };
						constexpr num_t Bottom{ 0 };
					}
					namespace Padding {
						constexpr num_t Left{ 0 };
						constexpr num_t Top{ push(1) };
						constexpr num_t Right{ Advance::Left };
						constexpr num_t Bottom{ push(1) };
					}
				}
				namespace Vban {
					constexpr num_t Height{ Panel::Height };
					constexpr num_t Width{ Panel::Height * 39 / 22 };
				}
				namespace Knob {
					constexpr num_t Height{ Panel::Height };
					constexpr num_t Width{ Panel::Height };
				}
				namespace Plug {
					constexpr num_t Height{ push(19) };
					constexpr num_t Width{ push(41) };
				}
				namespace Mixer {
					namespace Block {
						namespace Advance {
							constexpr num_t Left{ 0 };
							constexpr num_t Top{ push(2) };
							constexpr num_t Right{ 0 };
							constexpr num_t Bottom{ 0 };
						}
						constexpr num_t Height{ Plug::Height + Advance::Top + Plug::Height };
						constexpr num_t Width{ Plug::Width };
					}
				}
				namespace Input {
					namespace Block {
						namespace Advance {
							constexpr num_t Left{ push(1) };
							constexpr num_t Top{ 0 };
							constexpr num_t Right{ 0 };
							constexpr num_t Bottom{ 0 };
						}
						constexpr num_t Height{ Panel::Height };
						constexpr num_t Width{ Knob::Width + 2 * (Advance::Left + Mixer::Block::Width) };
					}
					namespace Section {
						namespace Advance {
							constexpr num_t Left{ push(1) };
							constexpr num_t Top{ 0 };
							constexpr num_t Right{ 0 };
							constexpr num_t Bottom{ 0 };
						}
						constexpr num_t Height{ Panel::Height };
						constexpr num_t Width{ Input::Block::Width + Advance::Left + Input::Block::Width };
					}
				}
				namespace Output {
					namespace Section {
						namespace Advance {
							constexpr num_t Left{ push(1) };
							constexpr num_t Top{ 0 };
							constexpr num_t Right{ 0 };
							constexpr num_t Bottom{ 0 };
						}
						constexpr num_t Height{ Panel::Height };
						constexpr num_t Width{ Knob::Width + 3 * (Advance::Left + Knob::Width) };
					}
				}
				namespace Panel {
					constexpr num_t Width{
						Advance::Left + Vban::Width
						+ Advance::Left + Input::Section::Width
						+ Advance::Left + Output::Section::Width
					};
				}
			}
		}
	}
}

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace _ {
				namespace Controls {
					enum flags : size_t {
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
						typename TToolkit,
						typename TFocusTracker>
					::std::unique_ptr<
						Composition<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>
					> SubscribeCherry(
						TTimer &timer,
						Cherry &mixer,
						TToolkit &toolkit,
						TFocusTracker &focusTracker,
						::std::bitset<
							flags::offset
							+ Cherry::InputSize
							+ Cherry::OutputSize
						> const &enabled,
						vector_t const &paddingPoint,
						vector_t const &paddingVertex);
					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					inline ::std::unique_ptr<
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
							flags::offset
							+ TMixer::InputSize
							+ TMixer::OutputSize
						> const &enabled,
						vector_t const &paddingPoint,
						vector_t const &paddingVertex) {
						if constexpr (::std::is_same_v<TMixer, Cherry>) {
							return SubscribeCherry(
								timer, mixer, toolkit, focusTracker,
								enabled, paddingPoint, paddingVertex);
						} else {
							return nullptr;
						}
					};

					template<typename TMixer>
					class bag;

					template<typename TMixer>
					void PickAndUpdateStrip(TMixer &mixer, ::Voicemeeter::UI::States::StripKnob const &state);
					template<typename TMixer>
					void PickAndUpdatePlug(TMixer &mixer, ::Voicemeeter::UI::States::Plug const &state);

					namespace Policies {
						namespace Control {
							namespace Notifications {
								template<typename TMixer>
								class Vban final {
								public:
									using state_t = num_t;

									inline explicit Vban(TMixer &mixer)
										: _mixer{ mixer } {

									};
									Vban() = delete;
									Vban(Vban const &) = delete;
									inline Vban(Vban &&) = default;

									inline ~Vban() = default;

									Vban & operator=(Vban const &) = delete;
									Vban & operator=(Vban &&) = delete;

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
									using state_t = ::Voicemeeter::UI::States::StripKnob;

									inline explicit StripKnob(TMixer &mixer)
										: _mixer{ mixer } {

									};
									StripKnob() = delete;
									StripKnob(StripKnob const &) = delete;
									inline StripKnob(StripKnob &&) = default;

									inline ~StripKnob() = default;

									StripKnob & operator=(StripKnob const &) = delete;
									StripKnob & operator=(StripKnob &&) = delete;

									inline void operator()(state_t const &state) const {
										PickAndUpdateStrip<TMixer>(
											_mixer, state);
									};

								private:
									TMixer &_mixer;
								};

								template<typename TMixer>
								class Plug final {
								public:
									using state_t = ::Voicemeeter::UI::States::Plug;

									inline explicit Plug(TMixer &mixer)
										: _mixer{ mixer } {

									};
									Plug() = delete;
									Plug(Plug const &) = delete;
									inline Plug(Plug &&) = default;

									inline ~Plug() = default;

									Plug & operator=(Plug const &) = delete;
									Plug & operator=(Plug &&) = delete;

									inline void operator()(state_t const &state) const {
										PickAndUpdatePlug<TMixer>(
											_mixer, state);
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
										, _instant{ ::std::move(instant) } {

									};
									ToggleAnimation() = delete;
									ToggleAnimation(ToggleAnimation const &) = delete;
									inline ToggleAnimation(ToggleAnimation &&) = default;

									inline ~ToggleAnimation() = default;

									ToggleAnimation & operator=(ToggleAnimation const &) = delete;
									ToggleAnimation & operator=(ToggleAnimation &&) = delete;

									template<typename TControl>
									inline void operator()(TControl *that, state_t const &state) const {
										if (_on) {
											_animated(that, state);
										} else {
											_instant(that, state);
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
							template<typename ...Args>
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
							inline void Rescale(vector_t const &vertex) {
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
					using Vban = ::Voicemeeter::UI::Decorators::HitTest<
						::Voicemeeter::UI::Decorators::Interactivity::Point<
							::Voicemeeter::UI::Decorators::Interactivity::Carousel<
							::Voicemeeter::UI::Adapters::Interactivity::Controllers::Circular<
							::Voicemeeter::UI::Adapters::State<
								::Voicemeeter::UI::Adapters::Interactivity::Transparent<
								::Voicemeeter::UI::Adapters::Glyph<
									::Voicemeeter::UI::Adapters::Graphics::Animated<
										::Voicemeeter::UI::Adapters::Graphics::Context<
											::Voicemeeter::UI::Graphics::Bundles::Vban<TToolkit>,
											::Voicemeeter::UI::Policies::Control::Updates::Animated::VbanContext<TToolkit>>,
										TToolkit,
										::Voicemeeter::UI::Policies::Control::Updates::Animated::VbanFrame<TToolkit>,
										slice_t>,
									::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
								>>,
								num_t,
								Policies::Control::Notifications::Vban<TMixer>,
								Policies::Control::Updates::ToggleAnimation<
									::Voicemeeter::UI::Policies::Control::Updates::Animated::Vban<TToolkit>,
									::Voicemeeter::UI::Policies::Control::Updates::Vban<TToolkit>>
							>>>,
							TFocusTracker
						>>;
					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					using StripKnob = ::Voicemeeter::UI::Decorators::HitTest<
						::Voicemeeter::UI::Decorators::Interactivity::Grab<
							::Voicemeeter::UI::Decorators::Interactivity::StripKnob<
								::Voicemeeter::UI::Adapters::Interactivity::Controllers::StripKnob<
								::Voicemeeter::UI::Adapters::State<
									::Voicemeeter::UI::Adapters::Interactivity::Transparent<
									::Voicemeeter::UI::Adapters::Glyph<
										::Voicemeeter::UI::Adapters::Graphics::Animated<
											::Voicemeeter::UI::Adapters::Graphics::Context<
												::Voicemeeter::UI::Graphics::Bundles::Knob<TToolkit>,
												::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobContext<TToolkit>>,
											TToolkit,
											::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>,
											slice_t, slice_t>,
										::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
									>>,
									::Voicemeeter::UI::States::StripKnob,
									Policies::Control::Notifications::StripKnob<TMixer>,
									Policies::Control::Updates::ToggleAnimation<
										::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
										::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>
								>>,
								TTimer,
								::Voicemeeter::UI::Policies::Orientation::Directions::Axis>,
							TFocusTracker
						>>;
					template<
						typename TTimer,
						typename TMixer,
						typename TToolkit,
						typename TFocusTracker>
					using Plug = ::Voicemeeter::UI::Decorators::HitTest<
						::Voicemeeter::UI::Decorators::Interactivity::Point<
							::Voicemeeter::UI::Decorators::Interactivity::Carousel<
							::Voicemeeter::UI::Adapters::Interactivity::Controllers::Plug<
							::Voicemeeter::UI::Adapters::State<
								::Voicemeeter::UI::Adapters::Interactivity::Transparent<
								::Voicemeeter::UI::Adapters::Glyph<
									::Voicemeeter::UI::Adapters::Graphics::Animated<
										::Voicemeeter::UI::Adapters::Graphics::Context<
											::Voicemeeter::UI::Graphics::Bundles::Plug<TToolkit>,
											::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugContext<TToolkit>>,
										TToolkit,
										::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>,
										slice_t>,
									::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
								>>,
								::Voicemeeter::UI::States::Plug,
								Policies::Control::Notifications::Plug<TMixer>,
								Policies::Control::Updates::ToggleAnimation<
									::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
									::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>
							>>>,
							TFocusTracker
						>>;

					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryOutputSection = ::Voicemeeter::UI::Panels::Stack<
						::Voicemeeter::UI::Policies::Orientation::Directions::Axis,
						::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio,
						StripKnob<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							StripKnob<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryMixerBlock = ::Voicemeeter::UI::Panels::Stack<
						::Voicemeeter::UI::Policies::Orientation::Directions::Axis,
						::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio,
						Plug<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							Plug<
								TTimer,
								Cherry,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryInputBlock = ::Voicemeeter::UI::Panels::Stack<
						::Voicemeeter::UI::Policies::Orientation::Directions::Axis,
						::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio,
						StripKnob<
							TTimer,
							Cherry,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							CherryMixerBlock<
								TTimer,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							CherryMixerBlock<
								TTimer,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryInputSection = ::Voicemeeter::UI::Panels::Stack<
						::Voicemeeter::UI::Policies::Orientation::Directions::Axis,
						::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio,
						CherryInputBlock<
							TTimer,
							TToolkit,
							TFocusTracker>,
						Decorators::ToggleVisibility<
						::Voicemeeter::UI::Decorators::Padding<
							CherryInputBlock<
								TTimer,
								TToolkit,
								TFocusTracker>,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
						>>>;
					template<
						typename TTimer,
						typename TToolkit,
						typename TFocusTracker>
					using CherryFrontPanel = ::Voicemeeter::UI::Decorators::Padding<
						::Voicemeeter::UI::Panels::Stack<
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio,
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								Vban<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>,
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryInputSection<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>,
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryOutputSection<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>>,
						::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio>;

					template<typename ...TTokens>
					class caggregator {
					public:
						inline explicit caggregator(TTokens &&...tokens)
							: _tokens{ ::std::move(tokens) ... }
							, _callback{}
							, _levels(Zero, sizeof...(TTokens)) {

						};
						caggregator() = delete;
						caggregator(caggregator const &) = delete;
						inline caggregator(caggregator &&other)
							: _tokens{ ::std::move(other._tokens) }
							, _levels{ ::std::move(other._levels) } {
							if (other._callback) {
								on_maxLevel(
									::std::move(other._callback));
							}
						};

						inline ~caggregator() = default;

						caggregator & operator=(caggregator const &) = delete;
						caggregator & operator=(caggregator &&) = delete;

						template<typename Fn>
						void on_maxLevel(Fn &&callback) {
							_callback = ::std::forward<Fn>(callback);
							::std::apply([
								&levels = _levels,
								&callback = _callback
							](TTokens &...tokens)->void {
								size_t temp{ 0 };
								(tokens.on_level([
									&levels,
									&callback,
									i = temp++
								](num_t value)->void {
									num_t before{ max(levels) };
									levels[i] = value;
									num_t after{ max(levels) };
									if (before == after) {
										return;
									}
									callback(after);
								})
								, ...);
							}, _tokens);
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
					template<Cherry::Strips Target>
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
							Cherry::POStrip::token,
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
							caggregator8
						> _channels;
					};

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
						template<typename ...Args>
						inline Composition(
							bag<Cherry> &&tokens,
							Args &&...args)
							: CherryFrontPanel<TTimer, TToolkit, TFocusTracker>{ ::std::forward<Args>(args) ... }
							, _tokens{ ::std::move(tokens) } {

						};
						Composition() = delete;
						Composition(Composition const &) = delete;
						Composition(Composition &&) = delete;

						inline ~Composition() = default;

						Composition & operator=(Composition const &) = delete;
						Composition & operator=(Composition &&) = delete;

					private:
						bag<Cherry> _tokens;
					};

					template<
						typename TMixer,
						typename TMixer::Strips Target>
					inline void UpdateStrip(TMixer &mixer, ::Voicemeeter::UI::States::StripKnob const &state) {
						mixer.get_Strip<Target>()
							.set_Mute<bag<TMixer>>(state.toggle);
						mixer.get_Strip<Target>()
							.set_Gain<bag<TMixer>>(state.degree * 4 / 15);
					};
					template<>
					inline void PickAndUpdateStrip<Cherry>(Cherry &mixer, ::Voicemeeter::UI::States::StripKnob const &state) {
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
						case Cherry::Strips::A2:
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
						typename TMixer::Strips From, typename TMixer::Strips To>
					inline void UpdatePlug(TMixer &mixer, ::Voicemeeter::UI::States::Plug const &state) {
						mixer.set_Plug<bag<TMixer>,
							From, To>(state.toggle);
					};
					template<Cherry::Strips From>
					inline void PickAndUpdateCherryPlug(Cherry &mixer, ::Voicemeeter::UI::States::Plug const &state) {
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
					inline void PickAndUpdatePlug<Cherry>(Cherry &mixer, ::Voicemeeter::UI::States::Plug const &state) {
						switch (state.from) {
						case Cherry::Strips::P:
							PickAndUpdateCherryPlug<
								Cherry::Strips::P>(
								mixer, state);
							break;
						case Cherry::Strips::V:
							PickAndUpdateCherryPlug<
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
						state_t state{};
						state.target = Target;
						stripKnob.set_State(state);
						tokens.get_Strip<Target>()
							.on_gain([
								&stripKnob
							](num_t value)->void {
								state_t state{ stripKnob.get_State() };
								state.degree = value * 15 / 4;
								stripKnob.set_State(state);
							});
						tokens.get_Strip<Target>()
							.on_mute([
								&stripKnob
							](bool value)->void {
								state_t state{ stripKnob.get_State() };
								state.toggle = value;
								stripKnob.set_State(state);
							});
						tokens.get_Channels<Target>()
							.on_maxLevel([
								&stripKnob
							](num_t value)->void {
								state_t state{ stripKnob.get_State() };
								state.level = value;
								stripKnob.set_State(state);
							});
					};
					template<typename TStripKnob>
					inline void PickAndSubscribeCherryStripKnob(size_t target,
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
						state_t state{};
						state.from = From;
						state.to = To;
						plug.set_State(state);
						plug.set_Label(state.to);
						tokens.get_Mixer()
							.on_plug<From, To>([
								&plug
							](bool value)->void {
								state_t state{ plug.get_State() };
								state.toggle = value;
								plug.set_State(state);
							});
					};
					template<
						Cherry::Strips From,
						typename TPlug>
					inline void PickAndSubscribeCherryPlug(size_t to,
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
					inline void PickAndSubscribeCherryPlug(size_t from, size_t to,
						TPlug &plug, bag<Cherry> &tokens) {
						switch (from) {
						case Cherry::Strips::P:
							PickAndSubscribeCherryPlug<
								Cherry::Strips::P>(to,
								plug, tokens);
							break;
						case Cherry::Strips::V:
							PickAndSubscribeCherryPlug<
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
					> SubscribeCherry(
						TTimer &timer,
						Cherry &mixer,
						TToolkit &toolkit,
						TFocusTracker &focusTracker,
						::std::bitset<
							flags::offset
							+ Cherry::InputSize
							+ Cherry::OutputSize
						> const &enabled,
						vector_t const &paddingPoint,
						vector_t const &paddingVertex) {
						num_t enabledInputs{
							static_cast<num_t>(
								enabled.test(flags::offset + Cherry::Strips::P))
							+ enabled.test(flags::offset + Cherry::Strips::V)
						};
						num_t enabledOutputs{
							static_cast<num_t>(
								enabled.test(flags::offset + Cherry::Strips::A1))
							+ enabled.test(flags::offset + Cherry::Strips::A2)
							+ enabled.test(flags::offset + Cherry::Strips::B1)
							+ enabled.test(flags::offset + Cherry::Strips::B2)
						};
						auto vban = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								Vban<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							enabled.test(flags::vban),
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							0, 1, 1,
							Policies::Control::Notifications::Vban<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Vban<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Vban<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Vban<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Vban<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Vban::Width,
								::Voicemeeter::UI::Layouts::Cherry::Vban::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::VbanFrame<TToolkit>{ toolkit },
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
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Knob::Width,
								::Voicemeeter::UI::Layouts::Cherry::Knob::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							slice_t{ 6, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
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
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Knob::Width,
								::Voicemeeter::UI::Layouts::Cherry::Knob::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							slice_t{ 6, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
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
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Knob::Width,
								::Voicemeeter::UI::Layouts::Cherry::Knob::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							slice_t{ 6, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								StripKnob<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							timer,
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Knob::Width,
								::Voicemeeter::UI::Layouts::Cherry::Knob::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							slice_t{ 6, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o3 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								StripKnob<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							2 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							timer,
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Knob::Width,
								::Voicemeeter::UI::Layouts::Cherry::Knob::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							slice_t{ 6, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
							toolkit);
						auto o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								StripKnob<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							3 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							timer,
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							Policies::Control::Notifications::StripKnob<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnob<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::StripKnob<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Knob::Width,
								::Voicemeeter::UI::Layouts::Cherry::Knob::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 6, 1 },
							slice_t{ 6, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::StripKnobFrame<TToolkit>{ toolkit },
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
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
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
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i1o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							3 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
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
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
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
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						auto i2o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								Plug<
									TTimer,
									Cherry,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							3 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							focusTracker,
							Policies::Control::Notifications::Plug<Cherry>{ mixer },
							Policies::Control::Updates::ToggleAnimation<
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>,
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>>{
								enabled.test(flags::animated),
								::Voicemeeter::UI::Policies::Control::Updates::Animated::Plug<TToolkit>{ toolkit },
								::Voicemeeter::UI::Policies::Control::Updates::Plug<TToolkit>{ toolkit }
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Plug::Width,
								::Voicemeeter::UI::Layouts::Cherry::Plug::Height
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							toolkit,
							slice_t{ 0, 1, 1 },
							::Voicemeeter::UI::Policies::Control::Updates::Animated::PlugFrame<TToolkit>{ toolkit },
							toolkit);
						if (enabled.test(flags::vban)) {
							vban->set_FrameColor(toolkit.get_Theme()
								.Inactive);
						}
						if (1 < enabledInputs) {
							i2->set_FrameColor(toolkit.get_Theme()
								.Inactive);
							i2->set_LabelColor(toolkit.get_Theme()
								.Inactive);
							i2->set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
							if (3 < enabledOutputs) {
								i2o4->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i2o4->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
							if (2 < enabledOutputs) {
								i2o3->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i2o3->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
							if (1 < enabledOutputs) {
								i2o2->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i2o2->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
							if (0 < enabledOutputs) {
								i2o1->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i2o1->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
						}
						if (0 < enabledInputs) {
							i1->set_FrameColor(toolkit.get_Theme()
								.Inactive);
							i1->set_LabelColor(toolkit.get_Theme()
								.Inactive);
							i1->set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
							if (3 < enabledOutputs) {
								i1o4->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i1o4->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
							if (2 < enabledOutputs) {
								i1o3->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i1o3->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
							if (1 < enabledOutputs) {
								i1o2->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i1o2->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
							if (0 < enabledOutputs) {
								i1o1->set_FrameColor(toolkit.get_Theme()
									.Inactive);
								i1o1->set_LabelColor(toolkit.get_Theme()
									.Inactive);
							}
						}
						if (3 < enabledOutputs) {
							o4->set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o4->set_LabelColor(toolkit.get_Theme()
								.Inactive);
							o4->set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						if (2 < enabledOutputs) {
							o3->set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o3->set_LabelColor(toolkit.get_Theme()
								.Inactive);
							o3->set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						if (1 < enabledOutputs) {
							o2->set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o2->set_LabelColor(toolkit.get_Theme()
								.Inactive);
							o2->set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						if (0 < enabledOutputs) {
							o1->set_FrameColor(toolkit.get_Theme()
								.Inactive);
							o1->set_LabelColor(toolkit.get_Theme()
								.Inactive);
							o1->set_IndicatorColor(toolkit.get_Theme()
								.Neutral);
						}
						::std::array<size_t, 6>targets{
							::std::numeric_limits<size_t>::max(),
							::std::numeric_limits<size_t>::max(),
							::std::numeric_limits<size_t>::max(),
							::std::numeric_limits<size_t>::max(),
							::std::numeric_limits<size_t>::max(),
							::std::numeric_limits<size_t>::max()
						};
						{
							auto current = targets.begin();
							if (enabled.test(flags::offset + Cherry::Strips::P)) {
								*(current++) = Cherry::Strips::P;
							}
							if (enabled.test(flags::offset + Cherry::Strips::V)) {
								*(current++) = Cherry::Strips::V;
							}
						}
						{
							auto current = targets.begin() + 2;
							if (enabled.test(flags::offset + Cherry::Strips::A1)) {
								*(current++) = Cherry::Strips::A1;
							}
							if (enabled.test(flags::offset + Cherry::Strips::A2)) {
								*(current++) = Cherry::Strips::A2;
							}
							if (enabled.test(flags::offset + Cherry::Strips::B1)) {
								*(current++) = Cherry::Strips::B1;
							}
							if (enabled.test(flags::offset + Cherry::Strips::B2)) {
								*(current++) = Cherry::Strips::B2;
							}
						}
						bag<Cherry> tokens{ mixer };
						if (enabled.test(flags::vban)) {
							vban->set_State(0);
							tokens.get_Mixer()
								.on_vban([
									&control = *vban
								](bool value)->void {
									control.set_State(
										static_cast<num_t>(value));
								});
						}
						PickAndSubscribeCherryStripKnob(targets[0],
							*i1, tokens);
						PickAndSubscribeCherryStripKnob(targets[1],
							*i2, tokens);
						PickAndSubscribeCherryStripKnob(targets[2],
							*o1, tokens);
						PickAndSubscribeCherryStripKnob(targets[3],
							*o2, tokens);
						PickAndSubscribeCherryStripKnob(targets[4],
							*o3, tokens);
						PickAndSubscribeCherryStripKnob(targets[5],
							*o4, tokens);
						PickAndSubscribeCherryPlug(targets[0], targets[2],
							*i1o1, tokens);
						PickAndSubscribeCherryPlug(targets[0], targets[3],
							*i1o2, tokens);
						PickAndSubscribeCherryPlug(targets[0], targets[4],
							*i1o3, tokens);
						PickAndSubscribeCherryPlug(targets[0], targets[5],
							*i1o4, tokens);
						PickAndSubscribeCherryPlug(targets[1], targets[2],
							*i2o1, tokens);
						PickAndSubscribeCherryPlug(targets[1], targets[3],
							*i2o2, tokens);
						PickAndSubscribeCherryPlug(targets[1], targets[4],
							*i2o3, tokens);
						PickAndSubscribeCherryPlug(targets[1], targets[5],
							*i2o4, tokens);
						auto i1o1o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Width,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 1 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1o1),
							::std::move(i1o2));
						auto i1o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							2 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Width,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 1 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1o3),
							::std::move(i1o4));
						auto i1o1o2o3o4 = ::std::make_unique<
							CherryInputBlock<
								TTimer,
								TToolkit,
								TFocusTracker>
						>(
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Width,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1),
							::std::move(i1o1o2),
							::std::move(i1o3o4));
						auto i2o1o2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Width,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 1 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i2o1),
							::std::move(i2o2));
						auto i2o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryMixerBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							2 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Width,
								::Voicemeeter::UI::Layouts::Cherry::Mixer::Block::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 1 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i2o3),
							::std::move(i2o4));
						auto i2o1o2o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryInputBlock<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							1 < enabledInputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Section::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Input::Section::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Section::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Input::Section::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Width,
								::Voicemeeter::UI::Layouts::Cherry::Input::Block::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i2),
							::std::move(i2o1o2),
							::std::move(i2o3o4));
						auto i1i2 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryInputSection<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledInputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Input::Section::Width,
								::Voicemeeter::UI::Layouts::Cherry::Input::Section::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							::std::move(i1o1o2o3o4),
							::std::move(i2o1o2o3o4));
						auto o1o2o3o4 = ::std::make_unique<
							Decorators::ToggleVisibility<
							::Voicemeeter::UI::Decorators::Padding<
								CherryOutputSection<
									TTimer,
									TToolkit,
									TFocusTracker>,
								::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio
							>>
						>(
							0 < enabledOutputs,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Left,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Top
							},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Right,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Advance::Bottom
							},
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Width,
								::Voicemeeter::UI::Layouts::Cherry::Output::Section::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
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
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Padding::Left,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Padding::Top
							} + paddingPoint,
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Padding::Right,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Padding::Top
							} + paddingVertex,
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
							vector_t{
								::Voicemeeter::UI::Layouts::Cherry::Panel::Width,
								::Voicemeeter::UI::Layouts::Cherry::Panel::Height
							},
							::Voicemeeter::UI::Policies::Orientation::Directions::Axis{ 0 },
							::Voicemeeter::UI::Policies::Size::Scales::PreserveRatio{},
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
