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

					virtual const ::std::valarray<double>& get_Position() const override final;
					virtual const ::std::valarray<double>& get_Size() const override final;
					virtual const ::std::valarray<double>& get_BaseSize() const override final;

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
					virtual void Move(const ::std::valarray<double>& point) override final;
					virtual void Rescale(const ::std::valarray<double>& vertex) override final;

				protected:
					Glyph(
						Canvas& canvas,
						const ::std::valarray<double>& baseVertex
					);

					inline const Canvas& get_Canvas() const {
						return m_canvas;
					}

				private:
					Canvas& m_canvas;
					::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					::std::valarray<double> m_baseVertex;
				};
			}
		}
	}
}