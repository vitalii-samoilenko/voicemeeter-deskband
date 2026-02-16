#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

#include <stdexcept>
#include <tuple>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct PreserveRatio {
						template<
							typename V,
							typename ...Vs>
						inline auto operator()(
							V const &dst, Vs &&...srcs) const {
							auto src = (srcs + ...);
							for (size_t i{ 0 }; i < dst.size(); ++i) {
								num_t nom{ dst[i] };
								num_t denom{ src[i] };
								if (any(dst < src * nom / denom)) {
									continue;
								}
								return ::std::make_tuple(
									srcs * nom / denom ...);
							}
							throw ::std::runtime_error{ "Scaling error" };
						};
					};
				}
			}
		}
	}
}

#endif
