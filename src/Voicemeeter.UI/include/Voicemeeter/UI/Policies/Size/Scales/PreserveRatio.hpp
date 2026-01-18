#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

#include <tuple>
#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct PreserveRatio {
						template<
							typename V,
							typename... Vs>
						inline ::std::tuple<Vs ...> operator()(
							V const &dst, Vs &&...srcs) const {
							auto src = srcs + ...;
							size_t scale_i{ 0 };
							{
								num_t r{ Inf };
								num_t rI{ -Inf };
								for (size_t i{ 0 }; i < dst.size(); ++i) {
									num_t nom{ dst[i] };
									num_t denom{ src[i] };
									if (nom < denom) {
										r = 0;
										num_t temp{ push(denom) / nom };
										if (rI < temp) {
											rI = temp;
											scale_i = i;
										}
									} else {
										num_t temp{ push(nom) / denom };
										if (temp < r) {
											r = temp;
											scale_i = i;
										}
									}
								}
							}
							return ::std::tuple<Args ...>{
								srcs * dst[scale_i] / src[scale_i] ...
							};
						};
					};
				}
			}
		}
	}
}

#endif
