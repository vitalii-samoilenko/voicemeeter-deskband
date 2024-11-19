#pragma once

#include <string>

#include "Voicemeeter.UI/States/Knob.h"

#include "../../../../Graphics/Glyphs/Knob.h"
#include "../../IUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Static {
							class Knob : IUpdate<Graphics::Glyphs::Knob, States::Knob> {
							public:
								Knob(
									const Graphics::Canvas& canvas,
									const ::std::wstring& label
								);
								Knob() = delete;
								Knob(const Knob&) = delete;
								Knob(Knob&&) = delete;

								~Knob() = default;

								Knob& operator=(const Knob&) = delete;
								Knob& operator=(Knob&&) = delete;

								virtual void Update(const States::Knob& state) override;

							private:
								const ::std::wstring m_label;
							};
						}
					}
				}
			}
		}
	}
}