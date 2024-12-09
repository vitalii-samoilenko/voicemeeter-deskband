#pragma once

#include "Voicemeeter.UI/Graphics/ICanvas.h"

#include "Palette.h"

using namespace ::Voicemeeter::UI::Graphics;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				class Canvas final : public ICanvas {
				public:
					explicit Canvas(
						const Palette& palette
					);
					Canvas() = delete;
					Canvas(const Canvas&) = delete;
					Canvas(Canvas&&) = delete;

					~Canvas() = default;

					Canvas& operator=(const Canvas&) = delete;
					Canvas& operator=(Canvas&&) = delete;

					virtual const ::std::valarray<double>& get_Position() const override;
					virtual const ::std::valarray<double>& get_Size() const override;

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
					virtual void Resize(const ::std::valarray<double>& vertex) override;

				private:
					const ::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					const Palette& m_palette;
				};
			}
		}
	}
}