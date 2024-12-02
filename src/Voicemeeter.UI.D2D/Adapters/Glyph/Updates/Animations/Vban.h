#pragma once

#include <chrono>
#include <type_traits>
#include <utility>

#include "../../../../Graphics/Glyphs/Vban.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						namespace Animations {
							template<typename TVban>
							class Vban : public Animation<1, UI::Policies::Size::Scales::Stretch, TVban, int> {
								static_assert(
									::std::is_base_of_v<Graphics::Glyphs::Vban, TVban>,
									"TVban must be derived from Vban");

								enum animation_vector : size_t {
									active = 0
								};

								using Animation = Animation<active + 1, UI::Policies::Size::Scales::Stretch, TVban, int>;

							public:
								template<typename... Args>
								explicit Vban(
									Args&& ...args
								) : Animation{ ::std::forward<Args>(args)... }
								  , m_baseVertex{
									  200LL * 1000LL
								  } {
									TVban::set_Color(TVban::get_Canvas()
										.get_Palette()
											.get_Theme()
												.Inactive);
								};
								Vban() = delete;
								Vban(const Vban&) = delete;
								Vban(Vban&&) = delete;

								~Vban() = default;

								Vban& operator=(const Vban&) = delete;
								Vban& operator=(Vban&&) = delete;

							protected:
								virtual const ::std::valarray<long long>& get_AnimationBaseSize() const override {
									return m_baseVertex;
								};

								virtual void OnUpdate(const int& state) override {
									Animation::get_Velocity()[active] = state
										? 1LL
										: -1LL;
								};
								virtual void OnFrame() override {
									FLOAT alpha{ static_cast<FLOAT>(Animation::get_AnimationSize()[active]) / m_baseVertex[active] };
									const ::D2D1::ColorF& from{
										TVban::get_Canvas()
											.get_Palette()
												.get_Theme()
													.Inactive
									};
									const ::D2D1::ColorF& to{
										TVban::get_Canvas()
											.get_Palette()
												.get_Theme()
													.SecondaryActive
									};
									TVban::set_Color(::D2D1::ColorF(
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