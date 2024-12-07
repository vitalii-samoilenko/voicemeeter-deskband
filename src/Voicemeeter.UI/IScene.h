#pragma once

#include "Traits/IInteractive.h"
#include "Traits/IResizable.h"
#include "Traits/IScalable.h"
#include "Traits/IVisible.h"

namespace Voicemeeter {
	namespace UI {
		class IScene
			: public Traits::IVisible
			, public Traits::IResizable
			, public Traits::IScalable
			, public Traits::IInteractive {
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