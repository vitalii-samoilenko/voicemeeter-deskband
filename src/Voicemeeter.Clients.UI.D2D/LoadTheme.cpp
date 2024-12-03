#include "Windows/Registry.h"

#include "Builder.h"

using namespace ::Voicemeeter::Clients::UI::D2D;

::Voicemeeter::UI::D2D::Graphics::Theme Voicemeeter::Clients::UI::D2D::LoadTheme() {
	::Voicemeeter::UI::D2D::Graphics::Theme theme{ ::Voicemeeter::UI::D2D::Graphics::Theme::Default() };
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