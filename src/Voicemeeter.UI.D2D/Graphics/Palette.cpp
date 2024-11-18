#include <limits>

#include "Windows/Wrappers.h"

#include "Canvas.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace ::Voicemeeter::UI::D2D::Graphics;

Palette::Palette(
	const Theme& theme,
	const Canvas& canvas
) : m_theme{ theme }
  , m_canvas{ canvas }
  , m_flatteringTolerance{ ::D2D1::ComputeFlatteningTolerance(::D2D1::IdentityMatrix(), 96.F, 96.F, 4.F) }
  , m_cpTextFormat{}
  , m_cpTextLayout{}
  , m_cpBrush{}
  , m_cpGeometry{} {

}

IDWriteTextFormat* Palette::get_pTextFormat(const ::std::wstring& fontFamily) const {
	::Microsoft::WRL::ComPtr<IDWriteTextFormat>& pTextFormat{
		m_cpTextFormat[fontFamily]
	};
	if (!pTextFormat) {
		::Windows::ThrowIfFailed(m_canvas.get_pDwFactory()
			->CreateTextFormat(
				fontFamily.c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				16,
				L"", //locale
				&pTextFormat
		), "Text format creation failed");
	}
	return pTextFormat.Get();
}
IDWriteTextLayout* Palette::get_pTextLayout(const ::std::wstring& text, const ::std::wstring& fontFamily) const {
	::Microsoft::WRL::ComPtr<IDWriteTextLayout>& pTextLayout{
		m_cpTextLayout[text]
	};
	if (!pTextLayout) {
		::Windows::ThrowIfFailed(m_canvas.get_pDwFactory()
			->CreateTextLayout(
				text.c_str(),
				static_cast<UINT32>(text.length()),
				get_pTextFormat(fontFamily),
				::std::numeric_limits<FLOAT>::max(),
				::std::numeric_limits<FLOAT>::max(),
				&pTextLayout
		), "Text layout creation failed");
	}
	return pTextLayout.Get();
}