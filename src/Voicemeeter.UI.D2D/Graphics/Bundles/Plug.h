#pragma once

#include "../Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Bundles {
					class Plug : public Bundle {
					public:
						explicit Plug(
							Graphics::Palette& palette,
							const ::std::wstring& label
						);
						Plug() = delete;
						Plug(const Plug&) = delete;
						Plug(Plug&&) = delete;

						~Plug() = default;

						Plug& operator=(const Plug&) = delete;
						Plug& operator=(Plug&&) = delete;

						inline void set_Label(const ::std::wstring& value) {
							m_label = value;
						}
						inline void set_Color(const ::D2D1::ColorF& value) {
							m_color = value;
						};

						virtual void Execute() override;

					private:
						::std::wstring m_label;
						::D2D1::ColorF m_color;
					};
				}
			}
		}
	}
}