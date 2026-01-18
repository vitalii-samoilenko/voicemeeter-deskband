#ifndef VOICEMEETER_UI_GRAPHICS_PALETTE_HPP
#define VOICEMEETER_UI_GRAPHICS_PALETTE_HPP

#include "wheel.hpp"

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
					gradient() = delete;
					inline gradient(gradient const &) = default;
					inline gradient(gradient &&) = default;

					inline ~gradient() = default;

					inline gradient & operator=(gradient const &) = default;
					inline gradient & operator=(gradient &&) = default;

					inline vector_t pick(num_t rI) const {
						float rF{ static_cast<float>(One) / rI };
						float rF1m{ 1.F - rF };
						::ok_color::RGB target{
							::ok_color::oklab_to_linear_srgb(
								::ok_color::Lab{
									_from.L * rF1m + _to.L * rF,
									_from.a * rF1m + _to.a * rF,
									_from.b * rF1m + _to.b * rF
								})
						};
						return vector_t{
							denormalize(
								::ok_color::srgb_transfer_function_inv(
									target.r)),
							denormalize(
								::ok_color::srgb_transfer_function_inv(
									target.r)),
							denormalize(
								::ok_color::srgb_transfer_function_inv(
									target.r)),
							(_fromA * (rI - One) + _toA) / rI
						};
					};

				private:
					friend Palette;

					::ok_color::Lab _from;
					::ok_color::Lab _to;
					num_t _fromA;
					num_t _toA;

					inline gradient(
						vector_t const &from,
						vector_t const &to)
						: _from{
							::ok_color::linear_srgb_to_oklab(
								::ok_color::RGB{
									::ok_color::srgb_transfer_function(
										normalize(from[0])),
									::ok_color::srgb_transfer_function(
										normalize(from[1])),
									::ok_color::srgb_transfer_function(
										normalize(from[2]))
								})
						}
						, _to{
							::ok_color::linear_srgb_to_oklab(
								::ok_color::RGB{
									::ok_color::srgb_transfer_function(
										normalize(to[0])),
									::ok_color::srgb_transfer_function(
										normalize(to[1])),
									::ok_color::srgb_transfer_function(
										normalize(to[2]))
								})
						}
						, _fromA{ from[3] }
						, _toA{ to[3] } {

					};

					inline static float normalize(num_t n) {
						return static_cast<float>(n) / push(255);
					};
					inline static int denormalize(float n) {
						return static_cast<num_t>(push(255) * n);
					};
				};

				inline gradient Interpolate(
					vector_t const &from, vector_t const &to) const {
					return gradient{ from, to };
				};
			};
		}
	}
}

#endif
