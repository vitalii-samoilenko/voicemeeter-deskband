#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

#include <tuple>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct PreserveRatio {
						template<typename... Args>
						inline ::std::tuple<Args ...> operator()(
							vector_t const &dst, Args &&...args) const {
							num_t scale{
								(dst * SCALING_FACTOR / (args + ...))
									.min()
							};
							return ::std::tuple<Args ...>{
								args * scale / SCALING_FACTOR ...
							};
						};
					};
				}
			}
		}
	}
}

#endif
