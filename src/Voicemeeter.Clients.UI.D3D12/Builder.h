#pragma once

#include <type_traits>
#include <unordered_set>
#include <vector>

#include "windows.h"

#include "Voicemeeter/Adapters/Multiclient/Cherry.h"
#include "Voicemeeter.UI/Direction.h"
#include "Voicemeeter.UI/Trackers/Focus.h"
#include "Voicemeeter.UI/Trackers/Input.h"
#include "Voicemeeter.UI.D3D12/Scene.h"
#include "Windows/Registry.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace D3D12 {
				template<typename TMixer>
				class Builder {
					static_assert(
						::std::is_same_v<Adapters::Multiclient::Cherry, TMixer>,
						"TMixer is not supported");

				public:
					inline Builder(
						HWND hWnd,
						HMODULE hModule,
						::Environment::IInputTracker& inputTracker,
						::Environment::ITimer& compositionTimer,
						TMixer& mixer
					) : m_hWnd{ hWnd }
					  , m_hModule{ hModule }
					  , m_inputTracker{ inputTracker }
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
						m_cIgnoredStrip.insert(id);
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
					::std::unique_ptr<::Voicemeeter::UI::D3D12::Scene> Build() {
						LoadOverrides();
						::std::unique_ptr<::Voicemeeter::UI::D3D12::Graphics::Palette> pPalette{
							new ::Voicemeeter::UI::D3D12::Graphics::Palette{
								m_theme, m_direction == ::Voicemeeter::UI::Direction::Right,
								m_hWnd, m_hModule
							}
						};
						::std::unique_ptr<::Voicemeeter::UI::Trackers::Focus> pFocusTracker{
							new ::Voicemeeter::UI::Trackers::Focus{}
						};
						::std::unique_ptr<::Voicemeeter::UI::Trackers::Input> pInputTracker{
							new ::Voicemeeter::UI::Trackers::Input{ m_inputTracker }
						};
						::std::unique_ptr<::Voicemeeter::UI::Graphics::ICanvas> pCanvas{
							new ::Voicemeeter::UI::D3D12::Graphics::Canvas{ *pPalette }
						};
						::std::unique_ptr<::Voicemeeter::UI::IComponent> pComposition{
							Compose(
								*pPalette,
								*pFocusTracker, *pInputTracker
							)
						};
						return ::std::unique_ptr<::Voicemeeter::UI::D3D12::Scene>{
							new ::Voicemeeter::UI::D3D12::Scene{
								pPalette,
								pInputTracker, pFocusTracker,
								pCanvas, pComposition
							}
						};
					};

				private:
					HWND m_hWnd;
					HMODULE m_hModule;
					::Environment::IInputTracker& m_inputTracker;
					::Environment::ITimer& m_compositionTimer;
					TMixer& m_mixer;
					::Voicemeeter::UI::Direction m_direction;
					::std::unordered_set<size_t> m_cIgnoredStrip;
					bool m_network;
					::Voicemeeter::UI::Cherry::Graphics::Theme m_theme;
					bool m_animations;
					::std::valarray<double> m_marginPoint;
					::std::valarray<double> m_marginVertex;

					inline void LoadOverrides() {
						using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;

						HKEY hKey{ HKEY_CURRENT_USER };
						::std::wstring subKey{ LR"(SOFTWARE\VoicemeeterDeskBand)" };
						::std::wstring themeSubKey{ subKey + LR"(\Theme)" };
						DWORD color{};
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Warning", color)) {
							m_theme.Warning[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.Warning[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.Warning[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Danger", color)) {
							m_theme.Danger[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.Danger[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.Danger[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"DarkGlass", color)) {
							m_theme.DarkGlass[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.DarkGlass[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.DarkGlass[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"LightGlass", color)) {
							m_theme.LightGlass[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.LightGlass[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.LightGlass[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"PrimaryActive", color)) {
							m_theme.PrimaryActive[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.PrimaryActive[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.PrimaryActive[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"SecondaryActive", color)) {
							m_theme.SecondaryActive[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.SecondaryActive[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.SecondaryActive[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Inactive", color)) {
							m_theme.Inactive[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.Inactive[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.Inactive[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Indicator", color)) {
							m_theme.Indicator[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.Indicator[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.Indicator[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"EqualizerLow", color)) {
							m_theme.EqualizerLow[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.EqualizerLow[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.EqualizerLow[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"EqualizerMedium", color)) {
							m_theme.EqualizerMedium[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.EqualizerMedium[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.EqualizerMedium[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"EqualizerHigh", color)) {
							m_theme.EqualizerHigh[RGBA::red] = (static_cast<UINT32>(color) >> (3 - RGBA::red) * 8) / 255.;
							m_theme.EqualizerHigh[RGBA::green] = (static_cast<UINT32>(color) >> (3 - RGBA::green) * 8) / 255.;
							m_theme.EqualizerHigh[RGBA::blue] = (static_cast<UINT32>(color) >> (3 - RGBA::blue) * 8) / 255.;
						}
						::std::wstring mixerSubKey{ subKey + LR"(\Mixer)" };
						DWORD network{ 0UL };
						if (::Windows::Registry::TryGetValue(hKey, mixerSubKey, L"Network", network)) {
							m_network = static_cast<bool>(network);
						}
						::std::vector<size_t> cIgnoredStrip{};
						if (::Windows::Registry::TryGetValue(hKey, mixerSubKey, L"IgnoredStrip", cIgnoredStrip)) {
							m_cIgnoredStrip = ::std::unordered_set<size_t>{ cIgnoredStrip.begin(), cIgnoredStrip.end() };
						}
						DWORD animations{ 0UL };
						if (::Windows::Registry::TryGetValue(hKey, subKey, L"Animations", animations)) {
							m_animations = static_cast<bool>(animations);
						}
					};
					::std::unique_ptr<::Voicemeeter::UI::IComponent> Compose(
						::Voicemeeter::UI::D3D12::Graphics::Palette& palette,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker
					);
				};
			}
		}
	}
}