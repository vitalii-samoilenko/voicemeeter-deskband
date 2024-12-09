#pragma once

#include <type_traits>
#include <unordered_set>
#include <vector>

#include "windows.h"

#include "Voicemeeter/Adapters/Multiclient/Cherry.h"
#include "Voicemeeter.UI/Direction.h"
#include "Voicemeeter.UI/Trackers/Focus.h"
#include "Voicemeeter.UI/Trackers/Input.h"
#include "Voicemeeter.UI.D2D/Scene.h"
#include "Voicemeeter.UI.D2D/Trackers/Dirty.h"
#include "Windows/Registry.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace D2D {
				template<typename TMixer>
				class Builder {
					static_assert(
						::std::is_same_v<Adapters::Multiclient::Cherry, TMixer>,
						"TMixer is not supported");

				public:
					inline Builder(
						HWND hWnd,
						::Environment::IDirtyTracker& dirtyTracker,
						::Environment::IInputTracker& inputTracker,
						::Environment::ITimer& compositionTimer,
						::Environment::ITimer& dirtyTimer,
						TMixer& mixer
					) : m_hWnd{ hWnd }
					  , m_dirtyTracker{ dirtyTracker }
					  , m_inputTracker{ inputTracker }
					  , m_compositionTimer{ compositionTimer }
					  , m_dirtyTimer{ dirtyTimer }
					  , m_mixer{ mixer }
					  , m_direction{ ::Voicemeeter::UI::Direction::Right }
					  , m_cIgnoredStrip{}
					  , m_network{ true }
					  , m_theme{ ::Voicemeeter::UI::D2D::Graphics::Theme::Dark() }
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
					inline Builder& WithTheme(const ::Voicemeeter::UI::D2D::Graphics::Theme& theme) {
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
					::std::unique_ptr<::Voicemeeter::UI::D2D::Scene> Build() {
						LoadOverrides();
						::std::unique_ptr<::Voicemeeter::UI::D2D::Trackers::Dirty> pDirtyTracker{
							new ::Voicemeeter::UI::D2D::Trackers::Dirty{ m_dirtyTracker, m_dirtyTimer }
						};
						::std::unique_ptr<::Voicemeeter::UI::Trackers::Focus> pFocusTracker{
							new ::Voicemeeter::UI::Trackers::Focus{}
						};
						::std::unique_ptr<::Voicemeeter::UI::Trackers::Input> pInputTracker{
							new ::Voicemeeter::UI::Trackers::Input{ m_inputTracker }
						};
						::std::unique_ptr<::Voicemeeter::UI::D2D::Graphics::Canvas> pCanvas{
							new ::Voicemeeter::UI::D2D::Graphics::Canvas{ m_hWnd, m_theme }
						};
						::std::unique_ptr<::Voicemeeter::UI::IComponent> pComposition{
							Compose(
								*pDirtyTracker, *pFocusTracker, *pInputTracker,
								*pCanvas
							)
						};
						return ::std::unique_ptr<::Voicemeeter::UI::D2D::Scene>{
							new ::Voicemeeter::UI::D2D::Scene{
								pDirtyTracker, pInputTracker, pFocusTracker,
								pCanvas, pComposition
							}
						};
					};

				private:
					HWND m_hWnd;
					::Environment::IDirtyTracker& m_dirtyTracker;
					::Environment::IInputTracker& m_inputTracker;
					::Environment::ITimer& m_compositionTimer;
					::Environment::ITimer& m_dirtyTimer;
					TMixer& m_mixer;
					::Voicemeeter::UI::Direction m_direction;
					::std::unordered_set<size_t> m_cIgnoredStrip;
					bool m_network;
					::Voicemeeter::UI::D2D::Graphics::Theme m_theme;
					bool m_animations;
					::std::valarray<double> m_marginPoint;
					::std::valarray<double> m_marginVertex;

					inline void LoadOverrides() {
						HKEY hKey{ HKEY_CURRENT_USER };
						::std::wstring subKey{ LR"(SOFTWARE\VoicemeeterDeskBand)" };
						::std::wstring themeSubKey{ subKey + LR"(\Theme)" };
						::Windows::Registry::TryGetValue(hKey, themeSubKey, L"FontFamily", m_theme.FontFamily);
						DWORD color{};
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Warning", color)) {
							m_theme.Warning = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Danger", color)) {
							m_theme.Danger = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"DarkGlass", color)) {
							m_theme.DarkGlass = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"LightGlass", color)) {
							m_theme.LightGlass = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"PrimaryActive", color)) {
							m_theme.PrimaryActive = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"SecondaryActive", color)) {
							m_theme.SecondaryActive = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Inactive", color)) {
							m_theme.Inactive = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"Indicator", color)) {
							m_theme.Indicator = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"EqualizerLow", color)) {
							m_theme.EqualizerLow = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"EqualizerMedium", color)) {
							m_theme.EqualizerMedium = ::D2D1::ColorF(static_cast<UINT32>(color));
						}
						if (::Windows::Registry::TryGetValue(hKey, themeSubKey, L"EqualizerHigh", color)) {
							m_theme.EqualizerHigh = ::D2D1::ColorF(static_cast<UINT32>(color));
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
						::Voicemeeter::UI::D2D::Trackers::Dirty& dirtyTracker,
						::Voicemeeter::UI::Trackers::Focus& focusTracker,
						::Voicemeeter::UI::Trackers::Input& inputTracker,
						::Voicemeeter::UI::D2D::Graphics::Canvas& canvas
					);
				};
			}
		}
	}
}