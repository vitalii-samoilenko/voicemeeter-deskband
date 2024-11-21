#pragma once

#include <string>

#include "../../../../Graphics/Glyphs/Plug.h"
#include "../../IUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Static {
							class Plug : public IUpdate<Graphics::Glyphs::Plug, int> {
							public:
								Plug(
									Graphics::Canvas& canvas,
									const ::std::wstring& label
								);
								Plug() = delete;
								Plug(const Plug&) = delete;
								Plug(Plug&&) = delete;

								~Plug() = default;

								Plug& operator=(const Plug&) = delete;
								Plug& operator=(Plug&&) = delete;

								virtual void Update(const int& state) override;
							};
						}
					}
				}
			}
		}
	}
}