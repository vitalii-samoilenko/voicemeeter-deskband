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

						virtual const ::linear_algebra::vectord& get_Position() const override final;
						virtual const ::linear_algebra::vectord& get_Size() const override final;
						virtual const ::linear_algebra::vectord& get_BaseSize() const override final;

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override final;
						virtual void Move(const ::linear_algebra::vectord& point) override final;
						virtual void Rescale(const ::linear_algebra::vectord& vertex) override final;

					protected:
						Glyph(
							Canvas& pCanvas,
							const ::linear_algebra::vectord& baseVertex
						);

						inline const Canvas& get_Canvas() const {
							return m_canvas;
						};

						virtual void OnDraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) = 0;

					private:
						Canvas& m_canvas;
						::linear_algebra::vectord m_point;
						::linear_algebra::vectord m_vertex;
						::linear_algebra::vectord m_baseVertex;
					};
				}
			}
		}
	}
}