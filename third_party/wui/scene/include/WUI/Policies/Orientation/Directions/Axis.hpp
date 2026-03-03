#ifndef WUI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP
#define WUI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP

#include "wheel.hpp"

namespace WUI {
	namespace Policies {
		namespace Orientation {
			namespace Directions {
				class Axis final {
				public:
					inline explicit Axis(size_t i)
						: _i{ i } {

					};
					Axis() = delete;
					Axis(Axis const &) = delete;
					inline Axis(Axis &&) = default;

					inline ~Axis() = default;

					Axis & operator=(Axis const &) = delete;
					Axis & operator=(Axis &&) = delete;

					template<typename V>
					inline auto operator()(V const &src) const {
						vec_t dst(Zero, size(src));
						sub(&dst, _i) = sub(src, _i);
						return dst;
					};

				private:
					size_t _i;
				};
			}
		}
	}
}

#endif
