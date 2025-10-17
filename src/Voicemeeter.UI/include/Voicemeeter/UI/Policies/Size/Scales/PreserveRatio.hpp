#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct PreserveRatio {
						inline ::std::valarray<double> operator()(
							::std::valarray<double> const &src,
							::std::valarray<double> const &dst) const {
							return ::std::valarray<double>(
								(dst / src).min(), src.size());
						};
					};
				}
			}
		}
	}
}

#endif
