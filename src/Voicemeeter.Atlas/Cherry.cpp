#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include "Cherry.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "windowscodecs")

using namespace ::Voicemeeter::Atlas;

struct ::Voicemeeter::Atlas::RenderTextW {
	Cherry& atlas;
	FLOAT scale;
	const ::std::wstring& text;

	inline void operator()() const {
		using Specification = CherrySpecification;

		::D2D1::Matrix3x2F base{};
		atlas.get_pRenderTarget()
			->GetTransform(&base);
		atlas.get_pRenderTarget()
			->SetTransform(
				::D2D1::Matrix3x2F::Scale(scale, scale)
				* base);

		::Microsoft::WRL::ComPtr<IDWriteTextLayout> pTextLayout{ nullptr };
		::Windows::ThrowIfFailed(atlas.get_pDwFactory()
			->CreateTextLayout(
				text.c_str(), static_cast<UINT32>(text.size()),
				atlas.get_pTextFormat(),
				::std::numeric_limits<FLOAT>::max(),
				::std::numeric_limits<FLOAT>::max(),
				&pTextLayout
			), "Text layout creation failed");
		DWRITE_TEXT_METRICS metrics{};
		::Windows::ThrowIfFailed(pTextLayout->GetMetrics(
			&metrics
		), "Text measurement failed");
		atlas.get_pRenderTarget()
			->DrawTextLayout(
				::D2D1::Point2F(
					(Specification::Block::Width - scale * metrics.width) / 2.F,
					(Specification::Block::Height - scale * metrics.height) / 2.F),
				pTextLayout.Get(),
				atlas.get_pBrush());
	};
};

Cherry::Cherry(
	bool horizontal,
	const ::std::valarray<double>& scale
) : Dynamic{
		CherrySpecification::Vban::Frame::Point::X, CherrySpecification::Vban::Frame::Point::Y,
		[this, scale = 2.F * CherrySpecification::Knob::Frame::Radius / (horizontal ? CherrySpecification::Vban::Frame::Height : CherrySpecification::Vban::Frame::Width)]()->void {
			using Specification = CherrySpecification::Vban;

			::D2D1::Matrix3x2F base{};
			get_pRenderTarget()
				->GetTransform(&base);
			get_pRenderTarget()
				->SetTransform(
					::D2D1::Matrix3x2F::Scale(scale, scale)
					* base);

			::Microsoft::WRL::ComPtr<IDWriteTextLayout> pTextLayout{ nullptr };
			::Windows::ThrowIfFailed(get_pDwFactory()
				->CreateTextLayout(
					L"V",
					1U,
					get_pTextFormat(),
					::std::numeric_limits<FLOAT>::max(),
					::std::numeric_limits<FLOAT>::max(),
					&pTextLayout
			), "Text layout creation failed");

			DWRITE_TEXT_METRICS metrics{};
			::Windows::ThrowIfFailed(pTextLayout->GetMetrics(
				&metrics
			), "Text measurement failed");

			get_pRenderTarget()
				->DrawRectangle(
					::D2D1::RectF(
						Specification::Frame::Stroke / 2.F,
						Specification::Frame::Stroke / 2.F,
						Specification::Frame::Width - Specification::Frame::Stroke / 2.F,
						Specification::Frame::Height - Specification::Frame::Stroke / 2.F),
					get_pBrush(), Specification::Frame::Stroke);
			get_pRenderTarget()
				->DrawTextLayout(
					::D2D1::Point2F(
						(Specification::Frame::Width - metrics.width) / 2.F,
						(Specification::Frame::Height - metrics.height) / 2.F),
					pTextLayout.Get(),
					get_pBrush());
			get_pRenderTarget()
				->FillRectangle(
					::D2D1::RectF(
						0.F,
						0.F,
						Specification::Side::Width, Specification::Frame::Height),
					get_pBrush());
			get_pRenderTarget()
				->FillRectangle(
					::D2D1::RectF(
						Specification::Frame::Width - Specification::Side::Width,
						0.F,
						Specification::Frame::Width,
						Specification::Frame::Height),
					get_pBrush());
		},
		CherrySpecification::Knob::Frame::Point::X, CherrySpecification::Knob::Frame::Point::Y,
		[this]()->void {
			using Specification = CherrySpecification::Knob;

			get_pRenderTarget()
				->DrawEllipse(
					::D2D1::Ellipse(
						::D2D1::Point2F(Specification::Frame::Radius, Specification::Frame::Radius),
						Specification::Frame::Radius - Specification::Frame::Stroke / 2.F,
						Specification::Frame::Radius - Specification::Frame::Stroke / 2.F),
					get_pBrush(), Specification::Frame::Stroke);
		},
		CherrySpecification::Knob::Indicator::Point::X, CherrySpecification::Knob::Indicator::Point::Y,
		[this]()->void {
			using Specification = CherrySpecification::Knob;

			get_pRenderTarget()
				->FillEllipse(
					::D2D1::Ellipse(
						::D2D1::Point2F(Specification::Indicator::Radius, Specification::Indicator::Radius),
						Specification::Indicator::Radius,
						Specification::Indicator::Radius),
					get_pBrush());
		},
		CherrySpecification::Plug::Frame::Point::X, CherrySpecification::Plug::Frame::Point::Y,
		[this, scale = CherrySpecification::Knob::Frame::Radius / (CherrySpecification::Plug::Frame::Height + 1.F)]()->void {
			using Specification = CherrySpecification::Plug;

			::D2D1::Matrix3x2F base{};
			get_pRenderTarget()
				->GetTransform(&base);
			get_pRenderTarget()
				->SetTransform(
					::D2D1::Matrix3x2F::Scale(scale, scale)
					* base);

			::Microsoft::WRL::ComPtr<ID2D1PathGeometry> pPath{ nullptr };
			::Windows::ThrowIfFailed(get_pD2dFactory()
				->CreatePathGeometry(
					&pPath
			), "Path creation failed");
			::Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink{ nullptr };
			::Windows::ThrowIfFailed(pPath->Open(
				&pSink
			), "Path initialization failed");

			pSink->BeginFigure(
				::D2D1::Point2F(Specification::Indicator::Start::X, Specification::Indicator::Start::Y),
				D2D1_FIGURE_BEGIN_FILLED
			);
			pSink->AddLine(
				::D2D1::Point2F(Specification::Indicator::Tip::X, Specification::Indicator::Tip::Y)
			);
			pSink->AddLine(
				::D2D1::Point2F(Specification::Indicator::End::X, Specification::Indicator::End::Y)
			);
			pSink->EndFigure(
				D2D1_FIGURE_END_CLOSED
			);

			::Windows::ThrowIfFailed(pSink->Close(
			), "Path finalization failed");

			get_pRenderTarget()
				->DrawRoundedRectangle(
					::D2D1::RoundedRect(
						::D2D1::RectF(
							Specification::Frame::Stroke / 2.F,
							Specification::Frame::Stroke / 2.F,
							Specification::Frame::Width - Specification::Frame::Stroke / 2.F,
							Specification::Frame::Height - Specification::Frame::Stroke / 2.F),
						Specification::Frame::Radius - Specification::Frame::Stroke / 2.F,
						Specification::Frame::Radius - Specification::Frame::Stroke / 2.F),
					get_pBrush(), Specification::Frame::Stroke);
			get_pRenderTarget()
				->FillGeometry(
					pPath.Get(),
					get_pBrush());
		},
		CherrySpecification::Plug::Label::Strip::Point::X + 2 % CherrySpecification::Width, CherrySpecification::Plug::Label::Strip::Point::Y + 2 / CherrySpecification::Width,
		RenderTextW{ *this, CherrySpecification::Knob::Frame::Radius / (CherrySpecification::Plug::Frame::Height + 2.F), L"A1" },
		CherrySpecification::Plug::Label::Strip::Point::X + 3 % CherrySpecification::Width, CherrySpecification::Plug::Label::Strip::Point::Y + 3 / CherrySpecification::Width,
		RenderTextW{ *this, CherrySpecification::Knob::Frame::Radius / (CherrySpecification::Plug::Frame::Height + 2.F), L"A2" },
		CherrySpecification::Plug::Label::Strip::Point::X + 4 % CherrySpecification::Width, CherrySpecification::Plug::Label::Strip::Point::Y + 4 / CherrySpecification::Width,
		RenderTextW{ *this, CherrySpecification::Knob::Frame::Radius / (CherrySpecification::Plug::Frame::Height + 2.F), L"B1" },
		CherrySpecification::Plug::Label::Strip::Point::X + 5 % CherrySpecification::Width, CherrySpecification::Plug::Label::Strip::Point::Y + 5 / CherrySpecification::Width,
		RenderTextW{ *this, CherrySpecification::Knob::Frame::Radius / (CherrySpecification::Plug::Frame::Height + 2.F), L"B2" },
		CherrySpecification::Knob::Label::Strip::Point::X + 0 % CherrySpecification::Width, CherrySpecification::Knob::Label::Strip::Point::Y + 0 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"P" },
		CherrySpecification::Knob::Label::Strip::Point::X + 1 % CherrySpecification::Width, CherrySpecification::Knob::Label::Strip::Point::Y + 1 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"V" },
		CherrySpecification::Knob::Label::Strip::Point::X + 2 % CherrySpecification::Width, CherrySpecification::Knob::Label::Strip::Point::Y + 2 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"A1" },
		CherrySpecification::Knob::Label::Strip::Point::X + 3 % CherrySpecification::Width, CherrySpecification::Knob::Label::Strip::Point::Y + 3 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"A2" },
		CherrySpecification::Knob::Label::Strip::Point::X + 4 % CherrySpecification::Width, CherrySpecification::Knob::Label::Strip::Point::Y + 4 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"B1" },
		CherrySpecification::Knob::Label::Strip::Point::X + 5 % CherrySpecification::Width, CherrySpecification::Knob::Label::Strip::Point::Y + 5 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"B2" },
		CherrySpecification::Knob::Label::Gain::Point::X + 0 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 0 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"0" },
		CherrySpecification::Knob::Label::Gain::Point::X + 1 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 1 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"1" },
		CherrySpecification::Knob::Label::Gain::Point::X + 2 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 2 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"2" },
		CherrySpecification::Knob::Label::Gain::Point::X + 3 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 3 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"3" },
		CherrySpecification::Knob::Label::Gain::Point::X + 4 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 4 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"4" },
		CherrySpecification::Knob::Label::Gain::Point::X + 5 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 5 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"5" },
		CherrySpecification::Knob::Label::Gain::Point::X + 6 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 6 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"6" },
		CherrySpecification::Knob::Label::Gain::Point::X + 7 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 7 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"7" },
		CherrySpecification::Knob::Label::Gain::Point::X + 8 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 8 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"8" },
		CherrySpecification::Knob::Label::Gain::Point::X + 9 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 9 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"9" },
		CherrySpecification::Knob::Label::Gain::Point::X + 10 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 10 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"10" },
		CherrySpecification::Knob::Label::Gain::Point::X + 11 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 11 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"11" },
		CherrySpecification::Knob::Label::Gain::Point::X + 12 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 12 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"12" },
		CherrySpecification::Knob::Label::Gain::Point::X + 13 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 13 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"13" },
		CherrySpecification::Knob::Label::Gain::Point::X + 14 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 14 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"14" },
		CherrySpecification::Knob::Label::Gain::Point::X + 15 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 15 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"15" },
		CherrySpecification::Knob::Label::Gain::Point::X + 16 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 16 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"16" },
		CherrySpecification::Knob::Label::Gain::Point::X + 17 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 17 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"17" },
		CherrySpecification::Knob::Label::Gain::Point::X + 18 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 18 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"18" },
		CherrySpecification::Knob::Label::Gain::Point::X + 19 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 19 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"19" },
		CherrySpecification::Knob::Label::Gain::Point::X + 20 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 20 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"20" },
		CherrySpecification::Knob::Label::Gain::Point::X + 21 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 21 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"21" },
		CherrySpecification::Knob::Label::Gain::Point::X + 22 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 22 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"22" },
		CherrySpecification::Knob::Label::Gain::Point::X + 23 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 23 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"23" },
		CherrySpecification::Knob::Label::Gain::Point::X + 24 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 24 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"24" },
		CherrySpecification::Knob::Label::Gain::Point::X + 25 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 25 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"25" },
		CherrySpecification::Knob::Label::Gain::Point::X + 26 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 26 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"26" },
		CherrySpecification::Knob::Label::Gain::Point::X + 27 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 27 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"27" },
		CherrySpecification::Knob::Label::Gain::Point::X + 28 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 28 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"28" },
		CherrySpecification::Knob::Label::Gain::Point::X + 29 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 29 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"29" },
		CherrySpecification::Knob::Label::Gain::Point::X + 30 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 30 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"30" },
		CherrySpecification::Knob::Label::Gain::Point::X + 31 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 31 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"31" },
		CherrySpecification::Knob::Label::Gain::Point::X + 32 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 32 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"32" },
		CherrySpecification::Knob::Label::Gain::Point::X + 33 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 33 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"33" },
		CherrySpecification::Knob::Label::Gain::Point::X + 34 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 34 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"34" },
		CherrySpecification::Knob::Label::Gain::Point::X + 35 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 35 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"35" },
		CherrySpecification::Knob::Label::Gain::Point::X + 36 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 36 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"36" },
		CherrySpecification::Knob::Label::Gain::Point::X + 37 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 37 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"37" },
		CherrySpecification::Knob::Label::Gain::Point::X + 38 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 38 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"38" },
		CherrySpecification::Knob::Label::Gain::Point::X + 39 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 39 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"39" },
		CherrySpecification::Knob::Label::Gain::Point::X + 40 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 40 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"40" },
		CherrySpecification::Knob::Label::Gain::Point::X + 41 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 41 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"41" },
		CherrySpecification::Knob::Label::Gain::Point::X + 42 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 42 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"42" },
		CherrySpecification::Knob::Label::Gain::Point::X + 43 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 43 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"43" },
		CherrySpecification::Knob::Label::Gain::Point::X + 44 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 44 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"44" },
		CherrySpecification::Knob::Label::Gain::Point::X + 45 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 45 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"45" },
		CherrySpecification::Knob::Label::Gain::Point::X + 46 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 46 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"46" },
		CherrySpecification::Knob::Label::Gain::Point::X + 47 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 47 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"47" },
		CherrySpecification::Knob::Label::Gain::Point::X + 48 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 48 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"48" },
		CherrySpecification::Knob::Label::Gain::Point::X + 49 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 49 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"49" },
		CherrySpecification::Knob::Label::Gain::Point::X + 50 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 50 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"50" },
		CherrySpecification::Knob::Label::Gain::Point::X + 51 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 51 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"51" },
		CherrySpecification::Knob::Label::Gain::Point::X + 52 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 52 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"52" },
		CherrySpecification::Knob::Label::Gain::Point::X + 53 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 53 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"53" },
		CherrySpecification::Knob::Label::Gain::Point::X + 54 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 54 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"54" },
		CherrySpecification::Knob::Label::Gain::Point::X + 55 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 55 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"55" },
		CherrySpecification::Knob::Label::Gain::Point::X + 56 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 56 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"56" },
		CherrySpecification::Knob::Label::Gain::Point::X + 57 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 57 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"57" },
		CherrySpecification::Knob::Label::Gain::Point::X + 58 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 58 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"58" },
		CherrySpecification::Knob::Label::Gain::Point::X + 59 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 59 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"59" },
		CherrySpecification::Knob::Label::Gain::Point::X + 60 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 60 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"60" },
		CherrySpecification::Knob::Label::Gain::Point::X + 61 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 61 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"61" },
		CherrySpecification::Knob::Label::Gain::Point::X + 62 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 62 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"62" },
		CherrySpecification::Knob::Label::Gain::Point::X + 63 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 63 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"63" },
		CherrySpecification::Knob::Label::Gain::Point::X + 64 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 64 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"64" },
		CherrySpecification::Knob::Label::Gain::Point::X + 65 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 65 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"65" },
		CherrySpecification::Knob::Label::Gain::Point::X + 66 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 66 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"66" },
		CherrySpecification::Knob::Label::Gain::Point::X + 67 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 67 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"67" },
		CherrySpecification::Knob::Label::Gain::Point::X + 68 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 68 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"68" },
		CherrySpecification::Knob::Label::Gain::Point::X + 69 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 69 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"69"},
		CherrySpecification::Knob::Label::Gain::Point::X + 70 % CherrySpecification::Width, CherrySpecification::Knob::Label::Gain::Point::Y + 70 / CherrySpecification::Width,
		RenderTextW{ *this, 1.F, L"70"},
		scale
	} {
	::Windows::ThrowIfFailed(get_pRenderTarget()
		->EndDraw(
	), "Render failed");

#ifndef NDEBUG
	WCHAR temp[MAX_PATH];
	::Windows::wGetTempPathW(MAX_PATH, temp);
	::std::wstring path{ temp };

	::Microsoft::WRL::ComPtr<IWICStream> pStream{ nullptr };
	::Windows::ThrowIfFailed(get_pWicFactory()
		->CreateStream(
			&pStream
	), "Stream creation failed");
	::Windows::ThrowIfFailed(pStream->InitializeFromFilename(
		path.append(L"Voicemeeter.Atlas.bmp").c_str(),
		GENERIC_WRITE
	), "Stream initialization failed");

	// Create and initialize WIC Bitmap Encoder.
	::Microsoft::WRL::ComPtr<IWICBitmapEncoder> pEncoder{ nullptr };
	::Windows::ThrowIfFailed(get_pWicFactory()
		->CreateEncoder(
			GUID_ContainerFormatBmp,
			nullptr,    // No preferred codec vendor.
			&pEncoder
	), "Encoder creation failed");
	::Windows::ThrowIfFailed(pEncoder->Initialize(
		pStream.Get(),
		WICBitmapEncoderNoCache
	), "Encoder initialization failed");

	// Create and initialize WIC Frame Encoder.
	::Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> pFrame{ nullptr };
	::Windows::ThrowIfFailed(pEncoder->CreateNewFrame(
		&pFrame,
		nullptr     // No encoder options.
	), "Frame creation failed");
	::Windows::ThrowIfFailed(pFrame->Initialize(
		nullptr
	), "Frame initialization failed");

	::Windows::ThrowIfFailed(pFrame->WriteSource(
		get_pBitmap(),
		NULL
	), "Frame writing failed");

	::Windows::ThrowIfFailed(pFrame->Commit(
	), "Frame commit failed");

	::Windows::ThrowIfFailed(pEncoder->Commit(
	), "Encode commit failed");

	// Flush all memory buffers to the next-level storage object.
	::Windows::ThrowIfFailed(pStream->Commit(
		STGC_DEFAULT
	), "Stream commit failed");
#endif // !NDEBUG
}