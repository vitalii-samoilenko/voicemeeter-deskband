#pragma once

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.UI/Graphics/ICanvas.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace GDI {
					class Canvas final : public ICanvas {
					public:
						Canvas() = default;
						Canvas(const Canvas&) = delete;
						Canvas(Canvas&&) = delete;

						~Canvas() = default;

						Canvas& operator=(const Canvas&) = delete;
						Canvas& operator=(Canvas&&) = delete;

						virtual void Redraw(linear_algebra::vector point, linear_algebra::vector vertex) override;
						virtual void Resize(linear_algebra::vector vertex) override;
					};
				}
			}
		}
	}
}