#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Bundles/Vban.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<typename TBundle>
						class Vban : public Animation<TBundle> {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Vban, TBundle>,
								"TVban must be derived from Vban");

							enum animation_vector : size_t {
								active = 0
							};

							using Base = Animation<TBundle>;

						public:
							template<typename... Args>
							explicit Vban(
								Args&& ...args
							) : Base{ {
									200LL * 1000LL * 1000LL
								}, ::std::forward<Args>(args)... } {

							};
							Vban() = delete;
							Vban(const Vban&) = delete;
							Vban(Vban&&) = delete;

							~Vban() = default;

							Vban& operator=(const Vban&) = delete;
							Vban& operator=(Vban&&) = delete;

						protected:
							virtual void OnFrame() override {
								::std::valarray<long long>& velocity{ Base::get_Velocity() };
								const ::std::valarray<long long>& vertex{ Base::get_AnimationSize() };
								const ::std::valarray<long long>& baseVertex{ Base::get_AnimationBaseSize() };
								FLOAT alpha{ static_cast<FLOAT>(vertex[active]) / baseVertex[active] };
								::D2D1::ColorF result{
									TBundle::get_Palette()
										.get_Theme()
											.Inactive
								};
								Graphics::Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.SecondaryActive,
									alpha);
								TBundle::set_Color(result);
								velocity[vertex == 0LL || vertex == baseVertex] = 0LL;
							};
						};
					}
				}
			}
		}
	}
}