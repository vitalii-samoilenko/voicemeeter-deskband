#pragma once

#include <string>

#include "estd/linear_algebra.h"

#include "../Canvas.h"
#include "../Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				namespace Glyphs {
					class Knob : public Glyph {
					public:
						Knob(
							Graphics::Canvas& canvas,
							const ::std::wstring& label
						);
						Knob() = delete;
						Knob(const Knob&) = delete;
						Knob(Knob&&) = delete;

						~Knob() = default;

						Knob& operator=(const Knob&) = delete;
						Knob& operator=(Knob&&) = delete;


						inline void set_Gain(FLOAT value) {
							m_gain = value;
						};
						inline void set_Level(FLOAT value) {
							m_level = value;
						};
						inline void set_Enabled(bool value) {
							m_enabled = value;
						};
						inline void set_Pinned(bool value) {
							m_pinned = value;
						};

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;

					private:
						::std::wstring m_label;
						FLOAT m_gain;
						FLOAT m_level;
						bool m_enabled;
						bool m_pinned;
					};
				}
			}
		}
	}
}