#pragma once

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.UI/Graphics/IGlyph.h"

#include "Canvas.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					class Glyph : public IGlyph {
					public:
						Glyph() = delete;
						Glyph(const Glyph&) = delete;
						Glyph(Glyph&&) = delete;

						~Glyph() = default;

						Glyph& operator=(const Glyph&) = delete;
						Glyph& operator=(Glyph&&) = delete;

						virtual linear_algebra::vector get_Position() const override final;
						virtual linear_algebra::vector get_Size() const override final;
						virtual linear_algebra::vector get_BaseSize() const override final;

						virtual void Redraw(linear_algebra::vector point, linear_algebra::vector vertex) override final;
						virtual void Move(linear_algebra::vector point) override final;
						virtual void Rescale(linear_algebra::vector vertex) override final;

					protected:
						Canvas& m_canvas;

						Glyph(
							Canvas& pCanvas,
							linear_algebra::vector baseVertex
						);

						virtual void OnDraw(linear_algebra::vector point, linear_algebra::vector vertex) = 0;

					private:
						linear_algebra::vector m_point;
						linear_algebra::vector m_vertex;
						linear_algebra::vector m_baseVertex;
					};
				}
			}
		}
	}
}