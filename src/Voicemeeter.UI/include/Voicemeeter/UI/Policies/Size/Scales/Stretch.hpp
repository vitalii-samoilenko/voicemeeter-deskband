#ifndef VOICEMEETER_UI_POLICIES_SIZE_SCALES_STRETCH_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_SCALES_STRETCH_HPP

#include <tuple>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Scales {
					struct Stretch {
						inline ::std::tuple<::std::valarray<int>> operator()(
							::std::valarray<int> const &dst,
							::std::valarray<int> const &src) const {
							return ::std::tuple<::std::valarray<int>>{ dst };
						};
						template<typename... Args>
						inline ::std::tuple<Args ...> operator()(
							::std::valarray<int> const &dst,
							Args &&...args) const {
							::std::valarray<int> scale{ dst * SCALING_FACTOR / (args + ...) };
							return ::std::tuple<Args ...>{ args * scale / SCALING_FACTOR ... };
						};
					};
				}
			}
		}
	}
}

#endif
