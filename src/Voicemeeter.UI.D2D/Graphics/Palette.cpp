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
  , m_cpTextFormat{}
  , m_cpTextLayout{}
  , m_cpBrush{}
  , m_cpGeometry{} {

}

IDWriteTextFormat* Palette::get_pTextFormat(const ::std::wstring& fontFamily) const {
	::Microsoft::WRL::ComPtr<IDWriteTextFormat>& pTextFormat{
		m_cpTextFormat[
			reinterpret_cast<void*>(&
				const_cast<::std::wstring&>(fontFamily))]
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
		m_cpTextLayout[
			reinterpret_cast<void*>(&
				const_cast<::std::wstring&>(text))]
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
ID2D1SolidColorBrush* Palette::get_pBrush(const ::D2D1::ColorF& color) const {
	::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush{
		m_cpBrush[
			reinterpret_cast<void*>(&
				const_cast<::D2D1::ColorF&>(color))]
	};
	if (!pBrush) {
		::Windows::ThrowIfFailed(m_canvas.get_pRenderTarget()
			->CreateSolidColorBrush(
				color,
				&pBrush
		), "Brush creation failed");
	}
	return pBrush.Get();
}
ID2D1PathGeometry* Palette::get_pGeometry(const ::std::type_info& type, bool& fresh) const {
	::Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry{
		m_cpGeometry[
			reinterpret_cast<void*>(&
				const_cast<::std::type_info&>(type))]
	};
	if (fresh = !pGeometry, fresh) {
		::Windows::ThrowIfFailed(m_canvas.get_pD2dFactory()
			->CreatePathGeometry(
				&pGeometry
		), "Geometry creation failed");
	}
	return pGeometry.Get();
}