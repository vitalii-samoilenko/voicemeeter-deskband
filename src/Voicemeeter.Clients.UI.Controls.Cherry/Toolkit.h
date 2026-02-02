#pragma once

#include <exception>
#include <memory>
#include <vector>

#include "Voicemeeter.UI/Decorators/Margin.h"
#include "Voicemeeter.UI/Decorators/RegionCheck.h"
#include "Voicemeeter.UI/Panels/Stack.h"
#include "Voicemeeter.UI/Policies/State/Changes/Checkbox.h"
#include "Voicemeeter.UI.Cherry/Decorators/Bundle/Animations/Knob.h"
#include "Voicemeeter.UI.Cherry/Decorators/Bundle/Animations/Plug.h"
#include "Voicemeeter.UI.Cherry/Decorators/Bundle/Animations/Vban.h"
#include "Voicemeeter.UI.Cherry/Graphics/Bundles/Knob.h"
#include "Voicemeeter.UI.Cherry/Graphics/Bundles/Plug.h"
#include "Voicemeeter.UI.Cherry/Graphics/Bundles/Vban.h"
#include "Voicemeeter.UI.Cherry/Graphics/Glyph.h"
#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Animations/Knob.h"
#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Animations/Plug.h"
#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Animations/Vban.h"
#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Knob.h"
#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Plug.h"
#include "Voicemeeter.UI.Cherry/Policies/Glyph/Updates/Vban.h"
#include "Voicemeeter.UI.Cherry/Decorators/Interactivity/Knob.h"
#include "Voicemeeter.UI.Cherry/Decorators/Interactivity/Plug.h"
#include "Voicemeeter.UI.Cherry/Decorators/Interactivity/Vban.h"
#include "Voicemeeter.UI.Cherry/Policies/State/Changes/Knob.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace Cherry {
				namespace Toolkit {
					template<::Voicemeeter::UI::Direction Direction>
					inline ::std::valarray<double> MarginPosition();
					template<>
					inline ::std::valarray<double> MarginPosition<::Voicemeeter::UI::Direction::Right>() {
						return ::std::valarray<double>{ 2., 0. };
					}
					template<>
					inline ::std::valarray<double> MarginPosition<::Voicemeeter::UI::Direction::Down>() {
						return ::std::valarray<double>{ 0., 2. };
					}

					template<
						typename TGlyph, typename TGlyphUpdate,
						typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeVban(
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::std::unique_ptr<TGlyph>& pGlyph,
						TMixer& mixer
					) {
						auto changeNotify = [&mixer](const int& state)->void {
							mixer.template set_Vban<TClient>(state);
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
						mixer.template get_Subscription<TClient>()
							.on_Vban(
								[&control = *pControl](bool value)->void {
									int src{ value };
									control.Set(src, false);
								});
						return pControl;
					}
					template<
						typename TInstrumentation,
						typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeVban(
						bool animations,
						::Voicemeeter::UI::Cherry::Graphics::Palette<TInstrumentation>& palette,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						TMixer& mixer
					) {
						using Bundle = ::Voicemeeter::UI::Cherry::Graphics::Bundles::Vban<
							TInstrumentation>;
						if (animations) {
							using Animation = ::Voicemeeter::UI::Cherry::Decorators::Bundle::Animations::Vban<
								TInstrumentation, Bundle>;
							using Glyph = ::Voicemeeter::UI::Cherry::Graphics::Glyph<
								TInstrumentation, Animation>;
							using GlyphUpdate = ::Voicemeeter::UI::Cherry::Policies::Glyph::Updates::Animations::Vban<
								TInstrumentation, Bundle, Animation, Glyph>;
							::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette } };
							return ComposeVban<Glyph, GlyphUpdate, TMixer, TClient>(
								focusTracker, inputTracker,
								pGlyph,
								mixer);
						} else {
							using Glyph = ::Voicemeeter::UI::Cherry::Graphics::Glyph<
								TInstrumentation, Bundle>;
							using GlyphUpdate = ::Voicemeeter::UI::Cherry::Policies::Glyph::Updates::Vban<
								TInstrumentation, Bundle, Glyph>;
							::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette } };
							return ComposeVban<Glyph, GlyphUpdate, TMixer, TClient>(
								focusTracker, inputTracker,
								pGlyph,
								mixer);
						}
					}

					template<
						::Voicemeeter::UI::Direction Direction, ::Voicemeeter::UI::Direction MarginDirection,
						typename TGlyph, typename TGlyphUpdate,
						typename TStrip, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
						bool first,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::Environment::ITimer& timer,
						::std::unique_ptr<TGlyph>& pGlyph,
						TStrip& strip, TMixer& mixer
					) {
						auto changeNotify = [&strip](const ::Voicemeeter::UI::Cherry::States::Knob& state)->void {
							strip.template set_Gain<TClient>(state.gain / 375.);
							strip.template set_Mute<TClient>(state.toggle);
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
						mixer.template get_Subscription<TClient>()
							.on_Gain(strip.get_Id(),
								[&control = *pControl](double value)->void {
									::Voicemeeter::UI::Cherry::States::Knob state{ control.get_State() };
									state.gain = static_cast<int>(value * 375.);
									control.Set(state, false);
								});
						mixer.template get_Subscription<TClient>()
							.on_Mute(strip.get_Id(),
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
							mixer.template get_Subscription<TClient>()
								.on_Level(line->get_Id(),
									[&control = *pControl, i = line - strip.begin()](double value)->void {
										::Voicemeeter::UI::Cherry::States::Knob state{ control.get_State() };
										state.level[i] = static_cast<int>(value * 10000.);
										control.Set(state, false);
									});
						}
						return pControl;
					}
					template<
						::Voicemeeter::UI::Direction Direction,
						typename TGlyph, typename TGlyphUpdate,
						typename TStrip, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
						::Voicemeeter::UI::Direction marginDirection,
						bool first,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::Environment::ITimer& timer,
						::std::unique_ptr<TGlyph>& pGlyph,
						TStrip& strip, TMixer& mixer
					) {
						switch (marginDirection) {
						case ::Voicemeeter::UI::Direction::Right:
							return ComposeKnob<Direction, ::Voicemeeter::UI::Direction::Right, TGlyph, TGlyphUpdate, TStrip, TMixer, TClient>(
								first,
								focusTracker, inputTracker,
								timer, pGlyph,
								strip, mixer
							);
						case ::Voicemeeter::UI::Direction::Down:
							return ComposeKnob<Direction, ::Voicemeeter::UI::Direction::Down, TGlyph, TGlyphUpdate, TStrip, TMixer, TClient>(
								first,
								focusTracker, inputTracker,
								timer, pGlyph,
								strip, mixer
							);
						default:
							throw ::std::exception{ "Direction is not supported" };
						}
					}
					template<
						typename TGlyph, typename TGlyphUpdate,
						typename TStrip, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
						::Voicemeeter::UI::Direction direction, ::Voicemeeter::UI::Direction marginDirection,
						bool first,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::Environment::ITimer& timer,
						::std::unique_ptr<TGlyph>& pGlyph,
						TStrip& strip, TMixer& mixer
					) {
						switch (direction) {
						case ::Voicemeeter::UI::Direction::Right:
							return ComposeKnob<::Voicemeeter::UI::Direction::Right, TGlyph, TGlyphUpdate, TStrip, TMixer, TClient>(
								marginDirection,
								first,
								focusTracker, inputTracker,
								timer, pGlyph,
								strip, mixer
							);
						case ::Voicemeeter::UI::Direction::Down:
							return ComposeKnob<::Voicemeeter::UI::Direction::Down, TGlyph, TGlyphUpdate, TStrip, TMixer, TClient>(
								marginDirection,
								first,
								focusTracker, inputTracker,
								timer, pGlyph,
								strip, mixer
							);
						default:
							throw ::std::exception{ "Direction is not supported" };
						}
					}
					template<
						typename TInstrumentation,
						typename TStrip, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposeKnob(
						bool animations, 
						::Voicemeeter::UI::Direction direction, ::Voicemeeter::UI::Direction marginDirection,
						bool first,
						::Voicemeeter::UI::Cherry::Graphics::Palette<TInstrumentation>& palette,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::Environment::ITimer& timer,
						TStrip& strip, TMixer& mixer
					) {
						size_t label{ ::Voicemeeter::UI::Cherry::Policies::State::Changes::Knob::ToLabel(strip.get_Id()) };
						using Bundle = ::Voicemeeter::UI::Cherry::Graphics::Bundles::Knob<
							TInstrumentation>;
						if (animations) {
							using Animation = ::Voicemeeter::UI::Cherry::Decorators::Bundle::Animations::Knob<
								TInstrumentation, Bundle>;
							using Glyph = ::Voicemeeter::UI::Cherry::Graphics::Glyph<
								TInstrumentation, Animation>;
							using GlyphUpdate = ::Voicemeeter::UI::Cherry::Policies::Glyph::Updates::Animations::Knob<
								TInstrumentation, Bundle, Animation, Glyph>;
							::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label }};
							return ComposeKnob<Glyph, GlyphUpdate, TStrip, TMixer, TClient>(
								direction, marginDirection,
								first,
								focusTracker, inputTracker,
								timer, pGlyph,
								strip, mixer);
						} else {
							using Glyph = ::Voicemeeter::UI::Cherry::Graphics::Glyph<
								TInstrumentation, Bundle>;
							using GlyphUpdate = ::Voicemeeter::UI::Cherry::Policies::Glyph::Updates::Knob<
								TInstrumentation, Bundle, Glyph>;
							::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label } };
							return ComposeKnob<Glyph, GlyphUpdate, TStrip, TMixer, TClient>(
								direction, marginDirection,
								first,
								focusTracker, inputTracker,
								timer, pGlyph,
								strip, mixer);
						}
					}

					template<
						::Voicemeeter::UI::Direction MarginDirection,
						typename TGlyph, typename TGlyphUpdate,
						typename TInput, typename TOutput, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePlug(
						bool first,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::std::unique_ptr<TGlyph>& pGlyph,
						TInput& input, TOutput& output, TMixer& mixer
					) {
						auto changeNotify = [&input, &output, &mixer](const int& state)->void {
							mixer.template set_Plug<TClient>(input, output, state);
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
						mixer.template get_Subscription<TClient>()
							.on_Plug(input.get_Id(), output.get_Id(),
								[&control = *pControl](bool value)->void {
									int state{ value };
									control.Set(state, false);
								});
						return pControl;
					}
					template<
						typename TGlyph, typename TGlyphUpdate,
						typename TInput, typename TOutput, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePlug(
						::Voicemeeter::UI::Direction marginDirection,
						bool first,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::std::unique_ptr<TGlyph>& pGlyph,
						TInput& input, TOutput& output, TMixer& mixer
					) {
						switch (marginDirection) {
						case ::Voicemeeter::UI::Direction::Right:
							return ComposePlug<::Voicemeeter::UI::Direction::Right, TGlyph, TGlyphUpdate, TInput, TOutput, TMixer, TClient>(
								first,
								focusTracker, inputTracker,
								pGlyph,
								input, output, mixer
							);
						case ::Voicemeeter::UI::Direction::Down:
							return ComposePlug<::Voicemeeter::UI::Direction::Down, TGlyph, TGlyphUpdate, TInput, TOutput, TMixer, TClient>(
								first,
								focusTracker, inputTracker,
								pGlyph,
								input, output, mixer
							);
						default:
							throw ::std::exception{ "Direction is not supported" };
						}
					}
					template<
						typename TInstrumentation,
						typename TInput, typename TOutput, typename TMixer, typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePlug(
						bool animations,
						::Voicemeeter::UI::Direction marginDirection,
						bool first,
						::Voicemeeter::UI::Cherry::Graphics::Palette<TInstrumentation>& palette,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						TInput& input, TOutput& output, TMixer& mixer
					) {
						size_t label{ ::Voicemeeter::UI::Cherry::Policies::State::Changes::Knob::ToLabel(output.get_Id()) };
						using Bundle = ::Voicemeeter::UI::Cherry::Graphics::Bundles::Plug<
							TInstrumentation>;
						if (animations) {
							using Animation = ::Voicemeeter::UI::Cherry::Decorators::Bundle::Animations::Plug<
								TInstrumentation, Bundle>;
							using Glyph = ::Voicemeeter::UI::Cherry::Graphics::Glyph<
								TInstrumentation, Animation>;
							using GlyphUpdate = ::Voicemeeter::UI::Cherry::Policies::Glyph::Updates::Animations::Plug<
								TInstrumentation, Bundle, Animation, Glyph>;
							::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label } };
							return ComposePlug<Glyph, GlyphUpdate, TInput, TOutput, TMixer, TClient>(
								marginDirection,
								first,
								focusTracker, inputTracker,
								pGlyph,
								input, output, mixer
							);
						} else {
							using Glyph = ::Voicemeeter::UI::Cherry::Graphics::Glyph<
								TInstrumentation, Bundle>;
							using GlyphUpdate = ::Voicemeeter::UI::Cherry::Policies::Glyph::Updates::Plug<
								TInstrumentation, Bundle, Glyph>;
							::std::unique_ptr<Glyph> pGlyph{ new Glyph{ palette, label } };
							return ComposePlug<Glyph, GlyphUpdate, TInput, TOutput, TMixer, TClient>(
								marginDirection,
								first,
								focusTracker, inputTracker,
								pGlyph,
								input, output, mixer
							);
						}
					}

					template<::Voicemeeter::UI::Direction Direction, ::Voicemeeter::UI::Direction MarginDirection>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
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
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
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
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
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
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
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
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> ComposePanel(
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
				}
			}
		}
	}
}