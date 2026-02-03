#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

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
							V const &dst, Vs const &&...srcs) const {
							num_t nom{ 0 };
							num_t denom{ 0 };
							{
								auto src = srcs + ...;
								num_t r{ Inf };
								num_t rI{ -Inf };
								for (size_t i{ 0 }; i < dst.size(); ++i) {
									num_t tom{ dst[i] };
									num_t tenom{ src[i] };
									if (tom < tenom) {
										r = 0;
										num_t temp{ push(tenom) / tom };
										if (rI < temp) {
											rI = temp;
											nom = tom;
											denom = tenom;
										}
									} else {
										num_t temp{ push(tom) / tenom };
										if (temp < r) {
											r = temp;
											nom = tom;
											denom = tenom;
										}
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
