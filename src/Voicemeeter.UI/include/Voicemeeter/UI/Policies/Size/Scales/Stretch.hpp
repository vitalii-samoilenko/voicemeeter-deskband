#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_STRETCH_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_STRETCH_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct Stretch {
						inline ::std::valarray<double> operator()(
							::std::valarray<double> const &src,
							::std::valarray<double> const &dst) const {
							return dst / src;
						};
					};
				}
			}
		}
	}
}

#endif
