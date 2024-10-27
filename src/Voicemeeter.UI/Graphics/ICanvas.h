#pragma once

#include "../Traits/IVisible.h"
#include "../Traits/IResizable.h"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			class ICanvas
				: public Traits::IVisible
				, public Traits::IResizable {
			public:
				ICanvas(const ICanvas&) = delete;
				ICanvas(ICanvas&&) = delete;

				virtual ~ICanvas() = default;

				ICanvas& operator=(const ICanvas&) = delete;
				ICanvas& operator=(ICanvas&&) = delete;

			protected:
				ICanvas() = default;
			};
		}
	}
}