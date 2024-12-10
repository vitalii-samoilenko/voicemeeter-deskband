#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct PreserveRatio {
						inline ::std::valarray<double> operator()(const ::std::valarray<double>& dst, const ::std::valarray<double>& src) const {
							::std::valarray<double> result{ src / dst };
							result = result.min();
							return result;
						};
					};
				}
			}
		}
	}
}