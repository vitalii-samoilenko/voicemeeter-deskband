#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <chrono>
#include <string>

#include "Voicemeeter.UI/Trackers/IDirty.h"

#include "../../../../Graphics/Glyphs/Vban.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							class Vban : public Animation<1, Graphics::Glyphs::Vban, int> {
							public:
								Vban(
									Trackers::IDirty& dirtyTracker,
									Graphics::Canvas& canvas
								);
								Vban() = delete;
								Vban(const Vban&) = delete;
								Vban(Vban&&) = delete;

								~Vban() = default;

								Vban& operator=(const Vban&) = delete;
								Vban& operator=(Vban&&) = delete;

							protected:
								virtual const ::std::valarray<long long>& get_AnimationBaseSize() const override {
									return g_baseVertex;
								};

								virtual void OnUpdate(const int& state) override;
								virtual void OnFrame() override;

							private:
								static const ::std::valarray<long long> g_baseVertex;
							};
						}
					}
				}
			}
		}
	}
}