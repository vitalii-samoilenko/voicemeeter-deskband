#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Bundles/Vban.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<
							typename TInstrumentation,
							typename TBundle>
						class Vban : public Animation<TInstrumentation, TBundle> {
							using Animation = Animation<TInstrumentation, TBundle>;
							using Bundle = Graphics::Bundles::Vban<TInstrumentation>;
							using Palette = Graphics::Palette<TInstrumentation>;

							static_assert(
								::std::is_base_of_v<Bundle, TBundle>,
								"TBundle must be derived from Vban");

						public:
							enum animation_vector : size_t {
								active = 0
							};

							template<typename... Args>
							explicit Vban(
								Args&& ...args
							) : Animation{ {
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
								::std::valarray<long long>& velocity{ Animation::get_Velocity() };
								const ::std::valarray<long long>& vertex{ Animation::get_AnimationSize() };
								const ::std::valarray<long long>& baseVertex{ Animation::get_AnimationBaseSize() };
								::std::valarray<double> result{
									TBundle::get_Palette()
										.get_Theme()
											.Inactive
								};
								Palette::Blend(result,
									TBundle::get_Palette()
										.get_Theme()
											.SecondaryActive,
									static_cast<double>(vertex[active]) / baseVertex[active]);
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