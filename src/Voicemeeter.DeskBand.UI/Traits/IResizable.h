#pragma once

#include "linear_algebra.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Traits {
				class IResizable {
				public:
					IResizable() = delete;
					IResizable(const IResizable&) = delete;
					IResizable(IResizable&&) = delete;

					~IResizable() = delete;

					IResizable& operator=(const IResizable&) = delete;
					IResizable& operator=(IResizable&&) = delete;

					virtual void Resize(linear_algebra::vector size) = 0;
				};
			}
		}
	}
}