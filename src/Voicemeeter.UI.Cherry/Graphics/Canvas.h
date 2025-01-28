#pragma once

#include "Voicemeeter.UI/Graphics/ICanvas.h"

#include "Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Graphics {
				template<typename TInstrumentation>
				class Canvas final : public UI::Graphics::ICanvas {
					using Palette = Palette<TInstrumentation>;

				public:
					inline explicit Canvas(
						const Palette& palette
					) : m_point{ 0., 0. }
					  , m_vertex{ 0., 0. }
					  , m_palette{ palette } {

					};
					Canvas() = delete;
					Canvas(const Canvas&) = delete;
					Canvas(Canvas&&) = delete;

					~Canvas() = default;

					Canvas& operator=(const Canvas&) = delete;
					Canvas& operator=(Canvas&&) = delete;

					virtual const ::std::valarray<double>& get_Position() const override {
						return m_point;
					};
					virtual const ::std::valarray<double>& get_Size() const override {
						return m_vertex;
					};

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
					virtual void Resize(const ::std::valarray<double>& vertex) override;

				private:
					const ::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					Palette& m_palette;
				};
			}
		}
	}
}