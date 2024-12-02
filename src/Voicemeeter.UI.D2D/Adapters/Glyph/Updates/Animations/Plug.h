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
							class Plug : public Animation<1, UI::Policies::Size::Scales::Stretch, TPlug, int> {
								static_assert(
									::std::is_base_of_v<Graphics::Glyphs::Plug, TPlug>,
									"TPlug must be derived from Plug");

								enum animation_vector : size_t {
									active = 0
								};

								using Animation = Animation<active + 1, UI::Policies::Size::Scales::Stretch, TPlug, int>;

							public:
								template<typename... Args>
								explicit Plug(
									const ::std::wstring& label,
									Args&& ...args
								) : Animation{ ::std::forward<Args>(args)... }
								  , m_baseVertex{
									  200LL * 1000LL
								  } {
									TPlug::set_Label(label);
									TPlug::set_Color(TPlug::get_Canvas()
										.get_Palette()
											.get_Theme()
												.Inactive);
								}
								Plug() = delete;
								Plug(const Plug&) = delete;
								Plug(Plug&&) = delete;

								~Plug() = default;

								Plug& operator=(const Plug&) = delete;
								Plug& operator=(Plug&&) = delete;

							protected:
								virtual const ::std::valarray<long long>& get_AnimationBaseSize() const override {
									return m_baseVertex;
								};

								virtual void OnUpdate(const int& state) override {
									TPlug::get_Velocity()[active] = state
										? 1LL
										: -1LL;
								};
								virtual void OnFrame() override {
									FLOAT alpha{ static_cast<FLOAT>(Animation::get_AnimationSize()[active]) / m_baseVertex[active] };
									const ::D2D1::ColorF& from{
										TPlug::get_Canvas()
											.get_Palette()
												.get_Theme()
													.Inactive
									};
									const ::D2D1::ColorF& to{
										TPlug::get_Canvas()
											.get_Palette()
												.get_Theme()
													.PrimaryActive
									};
									TPlug::set_Color(::D2D1::ColorF(
										from.r * (1.F - alpha) + to.r * alpha,
										from.g * (1.F - alpha) + to.g * alpha,
										from.b * (1.F - alpha) + to.b * alpha
									));
								};

							private:
								const ::std::valarray<long long> m_baseVertex;
							};
						}
					}
				}
			}
		}
	}
}