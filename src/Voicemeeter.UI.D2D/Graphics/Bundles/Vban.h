#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Bundles {
					class Vban : public Bundle {
					public:
						explicit Vban(
							Graphics::Palette& palette
						);
						Vban() = delete;
						Vban(const Vban&) = delete;
						Vban(Vban&&) = delete;

						~Vban() = default;

						Vban& operator=(const Vban&) = delete;
						Vban& operator=(Vban&&) = delete;

						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};

						virtual void Execute() override;

					private:
						::D2D1::ColorF m_color;
					};
				}
			}
		}
	}
}