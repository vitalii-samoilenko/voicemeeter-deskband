#include <vector>

#include "Windows/Wrappers.h"

#include "Canvas.h"

using namespace ::Voicemeeter::UI::D2D::Graphics;

Queue::Queue(
	::Environment::ITimer& timer,
	Canvas& canvas
) : m_canvas{ canvas }
  , m_cpGlyph{} {
	timer.Set(::std::chrono::milliseconds{ 30 },
		[this]()->bool {
			if (m_cpGlyph.empty()) {
				return true;
			}

			::std::vector<RECT> cRect{};

			m_canvas.get_pD2dDeviceContext()
				->BeginDraw();

			for (IGlyph* pGlyph : m_cpGlyph) {
				const ::std::valarray<double>& point{ pGlyph->get_Position() };
				const ::std::valarray<double>& vertex{ pGlyph->get_Size() };

				cRect.push_back({
					static_cast<LONG>(::std::floor(point[0])),
					static_cast<LONG>(::std::floor(point[1])),
					static_cast<LONG>(::std::ceil(point[0] + vertex[0])),
					static_cast<LONG>(::std::ceil(point[1] + vertex[1]))
				} );

				m_canvas.Redraw(point, vertex);
				pGlyph->Redraw(point, vertex);
			}

			::Windows::ThrowIfFailed(m_canvas.get_pD2dDeviceContext()
				->EndDraw(
			), "Render failed");

			DXGI_PRESENT_PARAMETERS params{
				static_cast<UINT>(cRect.size()), cRect.data(),
				nullptr,
				nullptr
			};
			::Windows::ThrowIfFailed(m_canvas.get_pDxgiSwapChain()
				->Present1(
					1, 0,
					&params
			), "Presentation failed");

			m_cpGlyph.clear();

			return true;
		});
}