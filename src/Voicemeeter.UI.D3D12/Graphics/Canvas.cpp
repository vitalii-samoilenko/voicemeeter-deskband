#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <cmath>

#include "Canvas.h"

using namespace ::Voicemeeter::UI::D3D12::Graphics;

void Canvas::Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
	using RGBA = ::Voicemeeter::UI::Cherry::Graphics::Theme::RGBA;

	const ::std::valarray<double>& color{
		m_palette.get_Theme()
			.Background
	};
}
void Canvas::Resize(const ::std::valarray<double>& vertex) {
	m_vertex[0] = ::std::max(8., vertex[0]);
	m_vertex[1] = ::std::max(8., vertex[1]);

	for (size_t frame{ 0 }; frame < Instrumentation::FRAME_COUNT; ++frame) {
		if (m_palette.get_Instrumentation()
			.get_pFence(frame)
				->GetCompletedValue() < m_palette.get_Instrumentation()
					.get_Count(frame)) {
			::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
				.get_pFence(frame)
					->SetEventOnCompletion(
						m_palette.get_Instrumentation()
							.get_Count(frame),
						m_palette.get_Instrumentation()
							.get_hEvent(frame)
			), "Event signaling failed");
			::Windows::wWaitForSingleObject(
				m_palette.get_Instrumentation()
					.get_hEvent(frame),
				INFINITE
			);
		}
		*m_palette.get_Instrumentation()
			.get_ppRenderTarget(frame) = nullptr;
	}

	::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
		.get_pSwapChain()
			->ResizeBuffers(
				0U,
				static_cast<UINT>(::std::ceil(m_vertex[0])),
				static_cast<UINT>(::std::ceil(m_vertex[1])),
				DXGI_FORMAT_UNKNOWN,
				0U
	), "Swap chain resize failed");

	for (size_t frame{ 0 }; frame < Instrumentation::FRAME_COUNT; ++frame) {
		::Windows::ThrowIfFailed(m_palette.get_Instrumentation()
			.get_pSwapChain()
				->GetBuffer(
					frame,
					IID_PPV_ARGS(m_palette.get_Instrumentation()
						.get_ppRenderTarget(frame))
		), "Failed to get swap chain buffer");
		m_palette.get_Instrumentation()
			.get_pD3dDevice()
				->CreateRenderTargetView(
					m_palette.get_Instrumentation()
						.get_pRenderTarget(frame),
					nullptr,
					m_palette.get_Instrumentation()
						.get_hvRenderTarget(frame));
	}
}