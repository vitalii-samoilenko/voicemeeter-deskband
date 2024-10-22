#pragma once

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TComponent>
				class IInteractivityPolicy {
					static_assert(estd::is_base_of<IComponent, TComponent>(),
						"TComponent must be derived from IComponent");

				public:
					IInteractivityPolicy(const IInteractivityPolicy&) = delete;
					IInteractivityPolicy(IInteractivityPolicy&&) = delete;

					virtual ~IInteractivityPolicy() = 0;

					IInteractivityPolicy& operator=(const IInteractivityPolicy&) = delete;
					IInteractivityPolicy& operator=(IInteractivityPolicy&&) = delete;

					virtual void MouseLDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const = 0;
					virtual void MouseRDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const = 0;
					virtual void MouseWheel(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point, int delta) const = 0;
					virtual void MouseMove(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const = 0;
					virtual void MouseLUp(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const = 0;

				protected:
					IInteractivityPolicy() = default;
				};
			}
		}
	}
}