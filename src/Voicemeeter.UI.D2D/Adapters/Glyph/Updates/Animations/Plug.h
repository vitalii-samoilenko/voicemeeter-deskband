#pragma once

#include <string>
#include <type_traits>
#include <utility>

#include "../../../../Graphics/Glyphs/Plug.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<typename TPlug>
							class Plug : public Animation<UI::Policies::Size::Scales::Stretch, TPlug, int> {
								static_assert(
									::std::is_base_of_v<Graphics::Glyphs::Plug, TPlug>,
									"TPlug must be derived from Plug");

								enum animation_vector : size_t {
									active = 0
								};

								using Animation = Animation<UI::Policies::Size::Scales::Stretch, TPlug, int>;

							public:
								template<typename... Args>
								explicit Plug(
									const ::std::wstring& label,
									Args&& ...args
								) : Animation{{
										200LL * 1000LL * 1000LL
									}, ::std::forward<Args>(args)... } {
									TPlug::set_Label(label);
									TPlug::set_Color(TPlug::get_Palette()
										.get_Theme()
											.Inactive);
								}
								Plug() = delete;
								Plug(const Plug&) = delete;
								Plug(Plug&&) = delete;

								~Plug() = default;

								Plug& operator=(const Plug&) = delete;
								Plug& operator=(Plug&&) = delete;

								inline void Update(const int& state) {
									Animation::get_Velocity()[active] = state
										? 1LL
										: -1LL;
									TPlug::get_DirtyTracker()
										.set_Dirty(*this, true);
								};

							protected:
								virtual void OnFrame() override {
									const ::std::valarray<long long>& vertex{ Animation::get_AnimationSize() };
									const ::std::valarray<long long>& baseVertex{ Animation::get_AnimationBaseSize() };
									FLOAT alpha{ static_cast<FLOAT>(vertex[active]) / baseVertex[active] };
									const ::D2D1::ColorF& from{
										TPlug::get_Palette()
											.get_Theme()
												.Inactive
									};
									const ::D2D1::ColorF& to{
										TPlug::get_Palette()
											.get_Theme()
												.PrimaryActive
									};
									TPlug::set_Color(::D2D1::ColorF(
										from.r * (1.F - alpha) + to.r * alpha,
										from.g * (1.F - alpha) + to.g * alpha,
										from.b * (1.F - alpha) + to.b * alpha
									));
								};
							};
						}
					}
				}
			}
		}
	}
}