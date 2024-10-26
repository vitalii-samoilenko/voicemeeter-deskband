#pragma once

#include <type_traits>

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "../Graphics/Canvas.h"
#include "../Graphics/Glyph.h"

using namespace ::Voicemeeter::DeskBand::Windows;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Decorators {
					template<typename TGlyph>
					class InstantRendering final : public TGlyph {
						static_assert(
							::std::is_base_of_v<Graphics::Glyph, TGlyph>,
							"TGlyph must be derived from Glyph");

					public:
						template<typename... Args>
						InstantRendering(
							Graphics::Canvas& canvas,
							Args... args
						) : TGlyph{ canvas, args... } {

						};
						InstantRendering() = delete;
						InstantRendering(const InstantRendering&) = delete;
						InstantRendering(InstantRendering&&) = delete;

						~InstantRendering() = default;

						InstantRendering& operator=(const InstantRendering&) = delete;
						InstantRendering& operator=(InstantRendering&&) = delete;

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override final {
							TGlyph::m_canvas.get_pRenderTarget()
								->BeginDraw();

							TGlyph::Redraw(point, vertex);

							ThrowIfFailed(TGlyph::m_canvas.get_pRenderTarget()
								->EndDraw(
							), "Render failed");
						};
					};
				}
			}
		}
	}
}