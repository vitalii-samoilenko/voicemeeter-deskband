#pragma once

#include "../IComponent.h"
#include "../Traits/IInteractive.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class IInput : public Traits::IInteractive {
			public:
				IInput(const IInput&) = delete;
				IInput(IInput&&) = delete;

				virtual ~IInput() = default;

				IInput& operator=(const IInput&) = delete;
				IInput& operator=(IInput&&) = delete;

				virtual const ::std::valarray<double>& get_Position() const = 0;
				virtual void set_Position(const ::std::valarray<double>& value) = 0;
				virtual bool get_Track(IComponent& component) const = 0;
				virtual void set_Track(IComponent& component, bool value) = 0;

			protected:
				IInput() = default;
			};
		}
	}
}