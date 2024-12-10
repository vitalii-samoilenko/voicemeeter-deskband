#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct Stretch {
						inline ::std::valarray<double> operator()(const ::std::valarray<double>& dst, const ::std::valarray<double>& src) const {
							return src / dst;
						};
					};
				}
			}
		}
	}
}