#include "checkboxscene.h"

#include "comerror.h"

using namespace Voicemeeter::DeskBand::Windows;

CheckboxScene::CheckboxScene(
	LPCWSTR lpszLabel
) : m_outer{}
  , m_inner{}
  , m_mark{ NULL }
  , m_lpszLabel{ lpszLabel }
  , m_lblSize{ static_cast<UINT32>(wcslen(lpszLabel)) }
  , m_lblBoundary{}
  , m_pLblFormat{ NULL } {

	}

void CheckboxScene::Resize(const Palette& rPalette) {
	D2D1_SIZE_F size{ rPalette.pTarget->GetSize() };
	if (size.width == 0.F || size.height == 0.F) {
		return;
	}
	D2D1_POINT_2F centre{ D2D1::Point2F(size.width / 2.F, size.height / 2.F) };

	m_outer = D2D1::Ellipse(centre, centre.x - size.width * 0.02F, centre.y - size.height * 0.02F);
	m_inner = D2D1::Ellipse(centre, centre.x - size.width * 0.06F, centre.y - size.height * 0.06F);

	D2D1_POINT_2F o{ D2D1::Point2F(centre.x - size.width * 0.33F, centre.y - size.height * 0.125F) };
	m_mark = NULL;
	ThrowIfFailed(rPalette.pFactory->CreatePathGeometry(
		&m_mark
	), "Geometry creation failed");
	CComPtr<ID2D1GeometrySink> pSink{ NULL };
	ThrowIfFailed(m_mark->Open(
		&pSink
	), "Failed to open geometry");
	pSink->BeginFigure(
		o,
		D2D1_FIGURE_BEGIN_FILLED
	);
	pSink->AddLine(D2D1::Point2F(o.x + size.width * 0.125F, o.y + size.height * 0.125F));
	pSink->AddLine(D2D1::Point2F(o.x, o.y + size.height * 0.25F));
	pSink->AddLine(o);
	pSink->EndFigure(
		D2D1_FIGURE_END_CLOSED
	);
	ThrowIfFailed(pSink->Close(

	), "Failed to close sink");

	m_lblBoundary = D2D1::RectF(0.F, 0.F, size.width, size.height);
	m_pLblFormat = NULL;
	ThrowIfFailed(rPalette.pDwFactory->CreateTextFormat(
		rPalette.lpszFontFamilyName,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		min(size.width, size.height) * 0.5F,
		L"",
		&m_pLblFormat
	), "Label format creation failed");
	m_pLblFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pLblFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

HRESULT CheckboxScene::Draw(const Palette& rPalette, bool checked) {
	rPalette.pTarget->BeginDraw();

	ID2D1SolidColorBrush* pOuter{ rPalette.pSub };
	ID2D1SolidColorBrush* pInner{ rPalette.pMain };
	ID2D1SolidColorBrush* pLabel{ rPalette.pSub };
	if (checked) {
		pOuter = rPalette.pActive;
		pInner = rPalette.pActive;
		pLabel = rPalette.pMain;
	}

	rPalette.pTarget->Clear(rPalette.pMain->GetColor());
	rPalette.pTarget->FillEllipse(m_outer, pOuter);
	rPalette.pTarget->FillEllipse(m_inner, pInner);
	rPalette.pTarget->FillGeometry(m_mark, pLabel);
	rPalette.pTarget->DrawTextW(
		m_lpszLabel, m_lblSize,
		m_pLblFormat,
		m_lblBoundary,
		pLabel
	);

	return rPalette.pTarget->EndDraw();
}

bool CheckboxScene::Inside(FLOAT x, FLOAT y) {
	FLOAT dx{ x - m_outer.point.x };
	FLOAT dy{ y - m_outer.point.y };

	return !(1.F < (dx * dx) / (m_outer.radiusX * m_outer.radiusX) + (dy * dy) / (m_outer.radiusY * m_outer.radiusY));
}