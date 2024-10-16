#pragma once

#include "estd/linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IResizable {
				public:
					IResizable(const IResizable&) = delete;
					IResizable(IResizable&&) = delete;

					IResizable& operator=(const IResizable&) = delete;
					IResizable& operator=(IResizable&&) = delete;

					virtual void Resize(linear_algebra::vector vertex) = 0;

				protected:
					IResizable() = default;

					~IResizable() = default;
				};
			}
		}
	}
}