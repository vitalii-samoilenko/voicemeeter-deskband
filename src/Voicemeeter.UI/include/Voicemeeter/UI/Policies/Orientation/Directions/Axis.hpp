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
						inline ::std::valarray<double> operator()(std::valarray<double> const &src) const {
							::std::valarray<double> dst(0, src.size());
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
