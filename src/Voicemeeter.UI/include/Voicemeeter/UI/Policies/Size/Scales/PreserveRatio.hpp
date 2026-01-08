#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_PRESERVERATIO_HPP

#include <tuple>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct PreserveRatio {
						template<typename... Args>
						inline ::std::tuple<Args ...> operator()(
							::std::valarray<int> const &dst,
							Args &&...args) const {
							::std::valarray<int> scale{ dst * SCALING_FACTOR / (args + ...) };
							scale = scale.min();
							return ::std::tuple<Args ...>{ args * scale / SCALING_FACTOR ... };
						};
					};
				}
			}
		}
	}
}

#endif
