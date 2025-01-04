#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Bundles {
					class Plug : public Bundle {
					public:
						Plug(
							Graphics::Palette& palette,
							size_t label
						);
						Plug() = delete;
						Plug(const Plug&) = delete;
						Plug(Plug&&) = delete;

						~Plug() = default;

						Plug& operator=(const Plug&) = delete;
						Plug& operator=(Plug&&) = delete;

						inline void set_Label(size_t value) {
							if (m_label == value) {
								return;
							}
							m_label = value;
							m_changed.set(label);
						}
						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};

						virtual void Execute() override;

					private:
						enum plug_flags : size_t {
							label = size + 1
						};

						size_t m_label;
						::D2D1::ColorF m_color;
						::std::valarray<double> m_maskPoint;
						::std::valarray<double> m_labelPoint;
						::std::valarray<double> m_labelVertex;
						::std::valarray<double> m_labelMaskPoint;
					};
				}
			}
		}
	}
}