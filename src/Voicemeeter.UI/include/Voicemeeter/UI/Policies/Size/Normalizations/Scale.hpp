#ifndef VOICEMEETER_UI_POLICIES_SIZE_NORMALIZATIONS_SCALE_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_NORMALIZATIONS_SCALE_HPP

#include <limits>
#include <memory>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Size {
				namespace Normalizations {
					struct Scale {
						template<typename TDirection, typename... TComponents>
						inline void operator()(
							TDirection &direction,
							size_t dimension,
							::std::unique_ptr<TComponents> &...components) const {
							::std::valarray<double> vertex{
								direction(::std::valarray<double>(
									::std::numeric_limits<double>::max(), dimension))
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
