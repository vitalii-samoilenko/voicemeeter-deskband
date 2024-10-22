#pragma once

#include "../Traits/IVisible.h"
#include "../Traits/IResizable.h"

using namespace Voicemeeter::DeskBand::UI::Traits;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				class ICanvas : public IVisible, public IResizable {
				public:
					ICanvas(const ICanvas&) = delete;
					ICanvas(ICanvas&&) = delete;

					virtual ~ICanvas() = 0;

					ICanvas& operator=(const ICanvas&) = delete;
					ICanvas& operator=(ICanvas&&) = delete;

				protected:
					ICanvas() = default;
				};
			}
		}
	}
}