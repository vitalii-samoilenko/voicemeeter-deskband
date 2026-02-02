#pragma once

#include <algorithm>
#include <type_traits>

#include "Toolkit.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"
#include "Windows/Registry.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace Cherry {
				template<
					typename TMixer,
					typename TInstrumentation>
				class Builder {
					static_assert(
						::std::is_same_v<Adapters::Multiclient::Cherry, TMixer>,
						"TMixer is not supported");

				public:
					Builder() = delete;
					Builder(const Builder&) = delete;
					Builder(Builder&&) = delete;

					inline ~Builder() = default;

					Builder& operator=(const Builder&) = delete;
					Builder& operator=(Builder&&) = delete;

					inline Builder& WithDirection(::Voicemeeter::UI::Direction direction) {
						m_direction = direction;
						return *this;
					};
					inline Builder& WithIgnoredStrip(size_t id) {
						m_cIgnoredStrip.push_back(id);
						return *this;
					};
					inline Builder& WithNetwork(bool network) {
						m_network = network;
						return *this;
					};
					inline Builder& WithTheme(const ::Voicemeeter::UI::Cherry::Graphics::Theme& theme) {
						m_theme = theme;
						return *this;
					};
					inline Builder& WithMarginPosition(const ::std::valarray<double>& point) {
						m_marginPoint = point;
						return *this;
					};
					inline Builder& WithMarginSize(const ::std::valarray<double>& vertex) {
						m_marginVertex = vertex;
						return *this;
					};

				protected:
					inline Builder(
						::Environment::IInputTracker& inputTracker,
						::Environment::ITimer& compositionTimer,
						TMixer& mixer
					) : m_inputTracker{ inputTracker }
					  , m_compositionTimer{ compositionTimer }
					  , m_mixer{ mixer }
					  , m_direction{ ::Voicemeeter::UI::Direction::Right }
					  , m_cIgnoredStrip{}
					  , m_network{ true }
					  , m_theme{ ::Voicemeeter::UI::Cherry::Graphics::Theme::Dark() }
					  , m_animations{ true }
					  , m_marginPoint{ 0., 0. }
					  , m_marginVertex{ 0., 0. } {

					};

					::Environment::IInputTracker& m_inputTracker;
					::Environment::ITimer& m_compositionTimer;
					TMixer& m_mixer;
					::Voicemeeter::UI::Direction m_direction;
					::std::vector<size_t> m_cIgnoredStrip;
					bool m_network;
					::Voicemeeter::UI::Cherry::Graphics::Theme m_theme;
					bool m_animations;
					::std::valarray<double> m_marginPoint;
					::std::valarray<double> m_marginVertex;

					inline void LoadOverrides() {
						HKEY hKey{ HKEY_CURRENT_USER };
						::std::wstring subKey{ LR"(SOFTWARE\VoicemeeterDeskBand)" };
						::std::wstring themeSubKey{ subKey + LR"(\Theme)" };
						LoadColor(m_theme.Warning, hKey, themeSubKey, L"Warning");
						LoadColor(m_theme.Danger, hKey, themeSubKey, L"Danger");
						LoadColor(m_theme.DarkGlass, hKey, themeSubKey, L"DarkGlass");
						LoadColor(m_theme.LightGlass, hKey, themeSubKey, L"LightGlass");
						LoadColor(m_theme.PrimaryActive, hKey, themeSubKey, L"PrimaryActive");
						LoadColor(m_theme.SecondaryActive, hKey, themeSubKey, L"SecondaryActive");
						LoadColor(m_theme.Inactive, hKey, themeSubKey, L"Inactive");
						LoadColor(m_theme.Indicator, hKey, themeSubKey, L"Indicator");
						LoadColor(m_theme.EqualizerLow, hKey, themeSubKey, L"EqualizerLow");
						LoadColor(m_theme.EqualizerMedium, hKey, themeSubKey, L"EqualizerMedium");
						LoadColor(m_theme.EqualizerHigh, hKey, themeSubKey, L"EqualizerHigh");
						::std::wstring mixerSubKey{ subKey + LR"(\Mixer)" };
						DWORD network{ 0UL };
						if (::Windows::Registry::TryGetValue(hKey, mixerSubKey, L"Network", network)) {
							m_network = static_cast<bool>(network);
						}
						::Windows::Registry::TryGetValue(hKey, mixerSubKey, L"IgnoredStrip", m_cIgnoredStrip);
						DWORD animations{ 0UL };
						if (::Windows::Registry::TryGetValue(hKey, subKey, L"Animations", animations)) {
							m_animations = static_cast<bool>(animations);
						}
					};
					template<typename TClient>
					inline ::std::unique_ptr<::Voicemeeter::UI::IComponent> Compose(
						::Voicemeeter::UI::Cherry::Graphics::Palette<TInstrumentation>& palette,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker
					) {
						::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpComponent{};
						if (m_network) {
							cpComponent.push_back(::std::move(
								Toolkit::ComposeVban<TInstrumentation, TMixer, TClient>(
									m_animations,
									palette,
									focusTracker, inputTracker,
									m_mixer)));
						}
						::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpBusComponent{};
						for (auto& input : m_mixer.get_PhysicalInput()) {
							if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), input.get_Id()) != m_cIgnoredStrip.end()) {
								continue;
							}
							cpBusComponent.push_back(::std::move(
								Toolkit::ComposeKnob<TInstrumentation, decltype(input), TMixer, TClient>(
									m_animations,
									m_direction, m_direction,
									cpBusComponent.empty(),
									palette,
									focusTracker, inputTracker,
									m_compositionTimer,
									input, m_mixer)));
							::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpPlugComponent{};
							for (auto& output : m_mixer.get_PhysicalOutput()) {
								if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), output.get_Id()) != m_cIgnoredStrip.end()) {
									continue;
								}
								cpPlugComponent.push_back(::std::move(
									Toolkit::ComposePlug<TInstrumentation, decltype(input), decltype(output), TMixer, TClient>(
										m_animations,
										::Voicemeeter::UI::Direction::Down,
										cpPlugComponent.empty(),
										palette,
										focusTracker, inputTracker,
										input, output, m_mixer)));
								if (cpPlugComponent.size() == 2) {
									cpBusComponent.push_back(::std::move(
										Toolkit::ComposePanel(
											::Voicemeeter::UI::Direction::Down, m_direction,
											cpBusComponent.empty(),
											cpPlugComponent)));
									cpPlugComponent.clear();
								}
							}
							for (auto& output : m_mixer.get_VirtualOutput()) {
								if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), output.get_Id()) != m_cIgnoredStrip.end()) {
									continue;
								}
								cpPlugComponent.push_back(::std::move(
									Toolkit::ComposePlug<TInstrumentation, decltype(input), decltype(output), TMixer, TClient>(
										m_animations,
										::Voicemeeter::UI::Direction::Down,
										cpPlugComponent.empty(),
										palette,
										focusTracker, inputTracker,
										input, output, m_mixer)));
								if (cpPlugComponent.size() == 2) {
									cpBusComponent.push_back(::std::move(
										Toolkit::ComposePanel(
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
								Toolkit::ComposePanel(
									m_direction, m_direction,
									cpComponent.empty(),
									cpBusComponent)));
							cpBusComponent.clear();
						}
						for (auto& input : m_mixer.get_VirtualInput()) {
							if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), input.get_Id()) != m_cIgnoredStrip.end()) {
								continue;
							}
							cpBusComponent.push_back(::std::move(
								Toolkit::ComposeKnob<TInstrumentation, decltype(input), TMixer, TClient>(
									m_animations,
									m_direction, m_direction,
									cpBusComponent.empty(),
									palette,
									focusTracker, inputTracker,
									m_compositionTimer,
									input, m_mixer)));
							::std::vector<::std::unique_ptr<::Voicemeeter::UI::IComponent>> cpPlugComponent{};
							for (auto& output : m_mixer.get_PhysicalOutput()) {
								if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), output.get_Id()) != m_cIgnoredStrip.end()) {
									continue;
								}
								cpPlugComponent.push_back(::std::move(
									Toolkit::ComposePlug<TInstrumentation, decltype(input), decltype(output), TMixer, TClient>(
										m_animations,
										::Voicemeeter::UI::Direction::Down,
										cpPlugComponent.empty(),
										palette,
										focusTracker, inputTracker,
										input, output, m_mixer)));
								if (cpPlugComponent.size() == 2) {
									cpBusComponent.push_back(::std::move(
										Toolkit::ComposePanel(
											::Voicemeeter::UI::Direction::Down, m_direction,
											cpBusComponent.empty(),
											cpPlugComponent)));
									cpPlugComponent.clear();
								}
							}
							for (auto& output : m_mixer.get_VirtualOutput()) {
								if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), output.get_Id()) != m_cIgnoredStrip.end()) {
									continue;
								}
								cpPlugComponent.push_back(::std::move(
									Toolkit::ComposePlug<TInstrumentation, decltype(input), decltype(output), TMixer, TClient>(
										m_animations,
										::Voicemeeter::UI::Direction::Down,
										cpPlugComponent.empty(),
										palette,
										focusTracker, inputTracker,
										input, output, m_mixer)));
								if (cpPlugComponent.size() == 2) {
									cpBusComponent.push_back(::std::move(
										Toolkit::ComposePanel(
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
								Toolkit::ComposePanel(
									m_direction, m_direction,
									cpComponent.empty(),
									cpBusComponent)));
							cpBusComponent.clear();
						}
						for (auto& input : m_mixer.get_PhysicalOutput()) {
							if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), input.get_Id()) != m_cIgnoredStrip.end()) {
								continue;
							}
							cpBusComponent.push_back(::std::move(
								Toolkit::ComposeKnob<TInstrumentation, decltype(input), TMixer, TClient>(
									m_animations,
									m_direction, m_direction,
									cpBusComponent.empty(),
									palette,
									focusTracker, inputTracker,
									m_compositionTimer,
									input, m_mixer)));
						}
						for (auto& input : m_mixer.get_VirtualOutput()) {
							if (::std::find(m_cIgnoredStrip.begin(), m_cIgnoredStrip.end(), input.get_Id()) != m_cIgnoredStrip.end()) {
								continue;
							}
							cpBusComponent.push_back(::std::move(
								Toolkit::ComposeKnob<TInstrumentation, decltype(input), TMixer, TClient>(
									m_animations,
									m_direction, m_direction,
									cpBusComponent.empty(),
									palette,
									focusTracker, inputTracker,
									m_compositionTimer,
									input, m_mixer)));
						}
						if (cpBusComponent.size()) {
							cpComponent.push_back(::std::move(
								Toolkit::ComposePanel(
									m_direction, m_direction,
									cpComponent.empty(),
									cpBusComponent)));
							cpBusComponent.clear();
						}
						return (0. < m_marginPoint).max() || (0. < m_marginVertex).max()
							? Toolkit::ComposePanel(
								m_direction,
								m_marginPoint, m_marginVertex,
								cpComponent)
							: Toolkit::ComposePanel(
								m_direction, m_direction,
								true,
								cpComponent);
					};

				private:
					inline void LoadColor(::std::valarray<double>& color, HKEY hKey, const ::std::wstring& subKey, const ::std::wstring& colorName) {
						using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;
						enum Mask : UINT32 {
							red   = 0b11111111000000000000000000000000,
							green = 0b00000000111111110000000000000000,
							blue  = 0b00000000000000001111111100000000
						};

						DWORD dColor{};
						if (::Windows::Registry::TryGetValue(hKey, subKey, colorName, dColor)) {
							color[RGBA::red] = (static_cast<UINT32>(dColor) & Mask::red ) / static_cast<double>(Mask::red);
							color[RGBA::green] = (static_cast<UINT32>(dColor) & Mask::green) / static_cast<double>(Mask::green);
							color[RGBA::blue] = (static_cast<UINT32>(dColor) & Mask::blue) / static_cast<double>(Mask::blue);
						}
					};
				};
			}
		}
	}
}