#pragma once

#include "../../../../Graphics/Glyphs/Vban.h"
#include "../../IUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Static {
							class Vban : IUpdate<Graphics::Glyphs::Vban, int> {
							public:
								explicit Vban(
									const Graphics::Canvas& canvas
								);
								Vban() = delete;
								Vban(const Vban&) = delete;
								Vban(Vban&&) = delete;

								~Vban() = default;

								Vban& operator=(const Vban&) = delete;
								Vban& operator=(Vban&&) = delete;

								virtual void Update(const int& state) override;
							};
						}
					}
				}
			}
		}
	}
}