#ifndef VOICEMEETER_UI_POLICIES_SIZE_NORMALIZATIONS_SCALE_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_NORMALIZATIONS_SCALE_HPP

#include <limits>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Normalizations {
					struct Scale {
						template<typename TDirection, typename... TComponents>
						inline void operator()(TDirection &direction, TComponents &...components) const {
							::std::valarray<int> vertex{
								direction(::std::valarray<int>{
									::std::numeric_limits<int>::max(),
									::std::numeric_limits<int>::max()
								})
							};
							((vertex[vertex < components->get_BaseSize()] = components->get_BaseSize()), ...);
							(components->Resize(vertex), ...);
						};
					};
				}
			}
		}
	}
}

#endif
