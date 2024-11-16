#pragma once

#include "Voicemeeter.UI/Graphics/IGlyph.h"

#include "Canvas.h"

using namespace ::Voicemeeter::UI::Graphics;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
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

					virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;
					virtual void Move(const ::linear_algebra::vectord& point) override final;
					virtual void Rescale(const ::linear_algebra::vectord& vertex) override final;

				protected:
					Canvas& m_canvas;

					Glyph(
						Canvas& canvas,
						const ::linear_algebra::vectord& baseVertex
					);

				private:
					::linear_algebra::vectord m_point;
					::linear_algebra::vectord m_vertex;
					::linear_algebra::vectord m_baseVertex;
				};
			}
		}
	}
}