#ifndef VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP
#define VOICEMEETER_UI_POLICIES_ORIENTATION_DIRECTIONS_AXIS_HPP

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Orientation {
				namespace Directions {
					template<size_t I>
					struct Axis {
						inline vector_t operator()(vector_t const &src) const {
							vector_t dst(0, src.size());
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
