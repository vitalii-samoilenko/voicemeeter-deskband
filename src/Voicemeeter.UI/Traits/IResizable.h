#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Traits {
			class IResizable {
			public:
				IResizable(const IResizable&) = delete;
				IResizable(IResizable&&) = delete;

				IResizable& operator=(const IResizable&) = delete;
				IResizable& operator=(IResizable&&) = delete;

				virtual void Resize(const ::std::valarray<double>& vertex) = 0;

			protected:
				IResizable() = default;

				~IResizable() = default;
			};
		}
	}
}