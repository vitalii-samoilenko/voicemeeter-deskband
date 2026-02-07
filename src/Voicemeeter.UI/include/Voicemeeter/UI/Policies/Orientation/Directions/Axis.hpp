#ifndef VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP
#define VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
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
							vector_t dst(0, src.size());
							dst[_i] = src[_i];
							return dst;
						};

					private:
						size_t _i;
					};
				}
			}
		}
	}
}

#endif
