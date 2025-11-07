#ifndef VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP
#define VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Orientation {
				namespace Directions {
					template<size_t I>
					struct Axis {
						inline ::std::valarray<int> operator()(std::valarray<int> const &src) const {
							::std::valarray<int> dst{ 0, 0 };
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
