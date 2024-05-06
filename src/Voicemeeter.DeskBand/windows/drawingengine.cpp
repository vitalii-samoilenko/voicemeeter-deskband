#include "drawingengine.h"

#include "wrappers.h"
#include "comerror.h"
#include "../messages.h"

using namespace Voicemeeter::DeskBand::Windows;

DrawingEngine::DrawingEngine(
	Style style
) : m_style{ style }
  , m_pFactory{ NULL }
  , m_pDwFactory{ NULL }
  , m_palettes{} {
		ThrowIfFailed(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&m_pFactory
		), "Factory creation failed");
		ThrowIfFailed(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDwFactory)
		), "DirectWrite factory creation failed");
	}

const Palette& DrawingEngine::Initialize(Window& rWnd) {
	RECT rc{};
	wGetClientRect(rWnd.get_hWnd(), &rc);
	
	Palette& rPalette{ m_palettes[&rWnd] = Palette{
		m_pFactory,
		m_pDwFactory,
		m_style.lpszFontFamilyName
	} };

	ThrowIfFailed(m_pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(rWnd.get_hWnd(), D2D1::SizeU(rc.right, rc.bottom)),
		&rPalette.pTarget
	), "Render target creation failed");
	ThrowIfFailed(rPalette.pTarget->CreateSolidColorBrush(
		m_style.Main,
		&rPalette.pMain
	), "Main brush creation failed");
	ThrowIfFailed(rPalette.pTarget->CreateSolidColorBrush(
		m_style.Sub,
		&rPalette.pSub
	), "Sub brush creation failed");
	ThrowIfFailed(rPalette.pTarget->CreateSolidColorBrush(
		m_style.Active,
		&rPalette.pActive
	), "Active brush creation failed");

	return rPalette;
}

const Palette& DrawingEngine::Resize(Window& rWnd) {
	RECT rc{};
	wGetClientRect(rWnd.get_hWnd(), &rc);

	const Palette& rPalette{ get_rPalette(rWnd) };

	ThrowIfFailed(get_rPalette(rWnd).pTarget->Resize(
		D2D1::SizeU(rc.right, rc.bottom)
	), "Resizing failed");

	return rPalette;
}