#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Bundles/Plug.h"
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
						class Plug : public Animation<TInstrumentation, TBundle> {
							using Animation = Animation<TInstrumentation, TBundle>;
							using Bundle = Graphics::Bundles::Plug<TInstrumentation>;
							using Palette = Graphics::Palette<TInstrumentation>;

							static_assert(
								::std::is_base_of_v<Bundle, TBundle>,
								"TBundle must be derived from Plug");

						public:
							enum animation_vector : size_t {
								active = 0
							};

							template<typename... Args>
							explicit Plug(
								Args&& ...args
							) : Animation{ {
									200LL * 1000LL * 1000LL
								}, ::std::forward<Args>(args)... } {

							}
							Plug() = delete;
							Plug(const Plug&) = delete;
							Plug(Plug&&) = delete;

							~Plug() = default;

							Plug& operator=(const Plug&) = delete;
							Plug& operator=(Plug&&) = delete;

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
											.PrimaryActive,
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