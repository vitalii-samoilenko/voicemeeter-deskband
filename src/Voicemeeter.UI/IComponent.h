#pragma once

#include "Traits/IInteractive.h"
#include "Traits/IMovable.h"
#include "Traits/IScalable.h"
#include "Traits/IVisible.h"

namespace Voicemeeter {
	namespace UI {
		class IComponent
			: public Traits::IVisible
			, public Traits::IScalable
			, public Traits::IMovable
			, public Traits::IInteractive {
		public:
			IComponent(const IComponent&) = delete;
			IComponent(IComponent&&) = delete;

			virtual ~IComponent() = default;

			IComponent& operator=(const IComponent&) = delete;
			IComponent& operator=(IComponent&&) = delete;

		protected:
			IComponent() = default;
		};
	}
}