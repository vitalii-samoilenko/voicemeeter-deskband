#pragma once

#include "Traits/IVisible.h"
#include "Traits/IResizable.h"
#include "Traits/IInteractive.h"

using namespace ::Voicemeeter::DeskBand::UI::Traits;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class IScene : public IVisible, public IResizable, public IInteractive {
			public:
				IScene(const IScene&) = delete;
				IScene(IScene&&) = delete;

				virtual ~IScene() = default;

				IScene& operator=(const IScene&) = delete;
				IScene& operator=(IScene&&) = delete;

			protected:
				IScene() = default;
			};
		}
	}
}