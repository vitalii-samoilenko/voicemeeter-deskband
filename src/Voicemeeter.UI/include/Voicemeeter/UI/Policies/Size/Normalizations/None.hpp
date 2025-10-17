#ifndef VOICEMEETER_UI_POLICIES_SIZE_NORMALIZATIONS_NONE_HPP
#define VOICEMEETER_UI_POLICIES_SIZE_NORMALIZATIONS_NONE_HPP

#include <memory>

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

						};
					};
				}
			}
		}
	}
}

#endif
