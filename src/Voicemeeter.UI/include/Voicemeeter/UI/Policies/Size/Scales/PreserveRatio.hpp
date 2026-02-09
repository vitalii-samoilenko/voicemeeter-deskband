#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

#include <limits>
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
							num_t nom{ 0 };
							num_t denom{ 0 };
							{
								auto src = (srcs + ...);
								double r{ ::std::numeric_limits<double>::infinity() };
								for (size_t i{ 0 }; i < dst.size(); ++i) {
									num_t tom{ dst[i] };
									num_t tenom{ src[i] };
									double temp{ static_cast<double>(tom) / tenom };
									if (temp < r) {
										r = temp;
										nom = tom;
										denom = tenom;
									}
								}
							}
							return ::std::make_tuple(
								srcs * nom / denom ...);
						};
					};
				}
			}
		}
	}
}

#endif
