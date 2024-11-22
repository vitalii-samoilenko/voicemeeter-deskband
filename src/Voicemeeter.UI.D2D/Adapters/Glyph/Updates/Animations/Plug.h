#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <chrono>
#include <string>

#include "Voicemeeter.UI/Trackers/IDirty.h"

#include "../../../../Graphics/Glyphs/Plug.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							class Plug : public Animation<1, Graphics::Glyphs::Plug, int> {
							public:
								Plug(
									Trackers::IDirty& dirtyTracker,
									Graphics::Canvas& canvas,
									const ::std::wstring& label
								);
								Plug() = delete;
								Plug(const Plug&) = delete;
								Plug(Plug&&) = delete;

								~Plug() = default;

								Plug& operator=(const Plug&) = delete;
								Plug& operator=(Plug&&) = delete;

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