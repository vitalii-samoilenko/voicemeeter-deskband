#ifndef VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP
#define VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Orientation {
				namespace Directions {
					template<size_t I>
					struct Axis {
						template<typename V>
						inline vector_t operator()(V const &src) const {
							vector_t dst(src.size(), 0);
							dst[I] = src[I];
							return dst;
						};
					};
				}
			}
		}
	}
}

#endif
