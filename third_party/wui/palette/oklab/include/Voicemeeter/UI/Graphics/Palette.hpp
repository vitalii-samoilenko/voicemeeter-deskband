#ifndef WUI_GRAPHICS_PALETTE_HPP
#define WUI_GRAPHICS_PALETTE_HPP

#include "wheel.hpp"

#include "ok_color.h"

namespace WUI {
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

				inline vec_t pick(num_t rI) const {
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
					return vec_t{
						Palette::denormalize(target.r),
						Palette::denormalize(target.g),
						Palette::denormalize(target.b),
						(_fromA * (rI - One) + push(_toA)) / rI
					};
				};

			private:
				friend class Palette;

				::ok_color::Lab _from;
				::ok_color::Lab _to;
				num_t _fromA;
				num_t _toA;

				inline gradient(
					vec_t const &from,
					vec_t const &to)
					: _from{
						::ok_color::linear_srgb_to_oklab(
							::ok_color::RGB{
								Palette::normalize(sub(from, 0)),
								Palette::normalize(sub(from, 1)),
								Palette::normalize(sub(from, 2))
							})
					}
					, _to{
						::ok_color::linear_srgb_to_oklab(
							::ok_color::RGB{
								Palette::normalize(sub(to, 0)),
								Palette::normalize(sub(to, 1)),
								Palette::normalize(sub(to, 2))
							})
					}
					, _fromA{ sub(from, 3) }
					, _toA{ sub(to, 3) } {

				};

			};

			inline gradient Interpolate(
				vec_t const &from, vec_t const &to) const {
				return gradient{ from, to };
			};
			inline vec_t Linearize(vec_t const &rgba) const {
				return vec_t{
					denormalize(
					::ok_color::srgb_transfer_function_inv(
					normalize(sub(rgba, 0)))),
					denormalize(
					::ok_color::srgb_transfer_function_inv(
					normalize(sub(rgba, 1)))),
					denormalize(
					::ok_color::srgb_transfer_function_inv(
					normalize(sub(rgba, 2)))),
					sub(rgba, 3)
				};
			};
			inline vec_t Premultiply(vec_t const &rgba) const {
				return vec_t{
					sub(rgba, 0) * sub(rgba, 3) / push(255),
					sub(rgba, 1) * sub(rgba, 3) / push(255),
					sub(rgba, 2) * sub(rgba, 3) / push(255),
					sub(rgba, 3)
				};
			};

		private:
			friend class gradient;

			inline static float normalize(num_t n) {
				return static_cast<float>(n) / push(255);
			};
			inline static int denormalize(float n) {
				return static_cast<num_t>(push(255) * n);
			};
		};
	}
}

#endif
