#ifndef VOICEMEETER_UI_GRAPHICS_PALETTE_HPP
#define VOICEMEETER_UI_GRAPHICS_PALETTE_HPP

#include <valarray>

#include "ok_color.h"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			class Palette final {
			public:
				inline Palette() = default;
				Palette(Palette const &) = delete;
				Palette(Palette &&) = delete;

				inline ~Palette() = default;

				Palette & operator=(Palette const &) = delete;
				Palette & operator=(Palette &&) = delete;

				class gradient final {
				public:
					inline gradient(
						::std::valarray<int> const &from,
						::std::valarray<int> const &to)
						: _from{
							::ok_color::linear_srgb_to_oklab(
								::ok_color::RGB{
									::ok_color::srgb_transfer_function(
										to_float(from[0])),
									::ok_color::srgb_transfer_function(
										to_float(from[1])),
									::ok_color::srgb_transfer_function(
										to_float(from[2]))
								})
						}
						, _to{
							::ok_color::linear_srgb_to_oklab(
								::ok_color::RGB{
									::ok_color::srgb_transfer_function(
										to_float(to[0])),
									::ok_color::srgb_transfer_function(
										to_float(to[1])),
									::ok_color::srgb_transfer_function(
										to_float(to[2]))
								})
						}
						, _fromA{ from[3] }
						, _toA{ to[3] } {

					};
					gradient() = delete;
					inline gradient(gradient const &) = default;
					inline gradient(gradient &&) = default;

					inline ~gradient() = default;

					inline gradient & operator=(gradient const &) = default;
					inline gradient & operator=(gradient &&) = default;

					inline ::std::valarray<int> Pick(int at) const {
						float atF{ to_float(at) };
						float atFi{ to_float(SCALING_FACTOR - at) };
						::ok_color::RGB target{
							::ok_color::oklab_to_linear_srgb(
								::ok_color::Lab{
									_from.L * atFi + _to.L * atF,
									_from.a * atFi + _to.a * atF,
									_from.b * atFi + _to.b * atF
								})
						};
						return ::std::valarray<int>{
							to_int(
								::ok_color::srgb_transfer_function_inv(
									target.r)),
							to_int(
								::ok_color::srgb_transfer_function_inv(
									target.r)),
							to_int(
								::ok_color::srgb_transfer_function_inv(
									target.r)),
							(_fromA * (SCALING_FACTOR - at) + _toA * at)
								/ SCALING_FACTOR
						};
					};

				private:
					::ok_color::Lab _from;
					::ok_color::Lab _to;
					int _fromA;
					int _toA;

					inline static float to_float(int n) {
						return static_cast<float>(n) / SCALING_FACTOR;
					};
					inline static int to_int(float n) {
						return static_cast<int>(n * SCALING_FACTOR);
					};
				};

				inline gradient Interpolate(
					::std::valarray<int> const &from,
					::std::valarray<int> const &to) const {
					return gradient{ from, to };
				};

			private:
			};
		}
	}
}

#endif
