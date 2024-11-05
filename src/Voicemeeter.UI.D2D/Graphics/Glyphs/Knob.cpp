#include <cmath>
#include <string>

#include "Windows/Wrappers.h"

#include <wrl/client.h>

#include "Knob.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace Voicemeeter::UI::D2D::Graphics::Glyphs;

Knob::Knob(
	Graphics::Canvas& canvas,
	const ::std::wstring& label
) : Glyph{ canvas, { 48, 48 } }
  , m_gain{}
  , m_leftLevel{}
  , m_rightLevel{}
  , m_enabled{}
  , m_pinned{}
  , m_label{ label } {

}

void Knob::Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) {
	Glyph::Redraw(point, vertex);

	float level{ ::std::max(m_leftLevel, m_rightLevel) };

	const Palette& palette{ m_canvas.get_Palette() };
	ID2D1SolidColorBrush* pBrush{
		(m_enabled
			? palette.get_pBrush(palette.get_Theme()
				.Warning)
			: m_pinned
				? 90.F < m_gain
					? palette.get_pBrush(palette.get_Theme()
						.Danger)
					: palette.get_pBrush(palette.get_Theme()
						.PrimaryActive)
				: level < 0.05F
					? palette.get_pBrush(palette.get_Theme()
						.Inactive)
					: level < 7.F
						? palette.get_pBrush(palette.get_Theme()
							.EqualizerLow)
						: level < 100.F
							? palette.get_pBrush(palette.get_Theme()
								.EqualizerMedium)
							: palette.get_pBrush(palette.get_Theme()
								.EqualizerHigh))
	};

	if (m_pinned) {
		::std::wstring label{ ::std::to_wstring(static_cast<int>(::std::abs((m_gain - 90) / 3.75))) };

		::Microsoft::WRL::ComPtr<IDWriteTextLayout> pLayout{ nullptr };
		::Windows::ThrowIfFailed(m_canvas.get_pDwFactory()
			->CreateTextLayout(
				label.c_str(),
				static_cast<UINT32>(label.length()),
				palette.get_pTextFormat(palette.get_Theme()
					.FontFamily),
				::std::numeric_limits<FLOAT>::max(),
				::std::numeric_limits<FLOAT>::max(),
				&pLayout
		), "Text layout creation failed");
		DWRITE_TEXT_METRICS metrics{};
		::Windows::ThrowIfFailed(pLayout->GetMetrics(
			&metrics
		), "Text measurement failed");

		m_canvas.get_pD2dDeviceContext()
			->DrawTextLayout(
				::D2D1::Point2F((48.F - metrics.width) / 2, (48.F - metrics.height) / 2),
				pLayout.Get(),
				pBrush);
	} else {
		IDWriteTextLayout* pLayout{
			palette.get_pTextLayout(
				m_label,
				palette.get_Theme()
					.FontFamily)
		};
		DWRITE_TEXT_METRICS metrics{};
		::Windows::ThrowIfFailed(pLayout->GetMetrics(
			&metrics
		), "Text measurement failed");

		m_canvas.get_pD2dDeviceContext()
			->DrawTextLayout(
				::D2D1::Point2F((48.F - metrics.width) / 2, (48.F - metrics.height) / 2),
				pLayout,
				pBrush);
	}

	m_canvas.get_pD2dDeviceContext()
		->DrawEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(24.F, 24.F), 22.5F, 22.5F),
			pBrush,
			3.F);
	D2D1_MATRIX_3X2_F base{};
	double scale{ get_Size().x / get_BaseSize().x };
	m_canvas.get_pD2dDeviceContext()
		->GetTransform(&base);
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(
			::D2D1::Matrix3x2F::Translation(24.F, 9.F)
			* ::D2D1::Matrix3x2F::Rotation(m_gain, ::D2D1::Point2F(24.F, 24.F))
			* base);
	m_canvas.get_pD2dDeviceContext()
		->FillEllipse(
			::D2D1::Ellipse(::D2D1::Point2F(0.F, 0.F), 2.75F, 2.75F),
			palette.get_pBrush(palette.get_Theme()
				.Indicator));
	m_canvas.get_pD2dDeviceContext()
		->SetTransform(base);
};