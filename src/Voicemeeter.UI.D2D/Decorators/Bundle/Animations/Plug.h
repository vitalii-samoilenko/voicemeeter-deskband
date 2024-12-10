#pragma once

#include <type_traits>
#include <utility>

#include "../../../Graphics/Bundles/Plug.h"
#include "../Animation.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				namespace Bundle {
					namespace Animations {
						template<typename TBundle>
						class Plug : public Animation<TBundle> {
							static_assert(
								::std::is_base_of_v<Graphics::Bundles::Plug, TBundle>,
								"TBundle must be derived from Plug");

							enum animation_vector : size_t {
								active = 0
							};

							using Base = Animation<TBundle>;

						public:
							template<typename... Args>
							explicit Plug(
								Args&& ...args
							) : Base{ {
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
											.PrimaryActive,
									alpha);
								TBundle::set_Color(result);
							};
						};
					}
				}
			}
		}
	}
}