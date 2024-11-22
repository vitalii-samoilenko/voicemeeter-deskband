#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <chrono>
#include <string>

#include "Voicemeeter.UI/States/Knob.h"
#include "Voicemeeter.UI/Trackers/IDirty.h"

#include "../../../../Graphics/Glyphs/Knob.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							class Knob : public Animation<7, Graphics::Glyphs::Knob, States::Knob> {
							public:
								Knob(
									Trackers::IDirty& dirtyTracker,
									Graphics::Canvas& canvas,
									const ::std::wstring& label
								);
								Knob() = delete;
								Knob(const Knob&) = delete;
								Knob(Knob&&) = delete;

								~Knob() = default;

								Knob& operator=(const Knob&) = delete;
								Knob& operator=(Knob&&) = delete;

							protected:
								virtual const ::std::valarray<long long>& get_AnimationBaseSize() const override {
									return g_baseVertex;
								};

								virtual void OnUpdate(const States::Knob& state) override;
								virtual void OnFrame() override;

							private:
								static const ::std::valarray<long long> g_baseVertex;
								::std::wstring m_label;
								int m_gain;
							};
						}
					}
				}
			}
		}
	}
}