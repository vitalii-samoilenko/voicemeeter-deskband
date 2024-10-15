#pragma once

#include "type_traits.h"
#include "linear_algebra.h"

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
					IInteractivityPolicy() = delete;
					IInteractivityPolicy(const IInteractivityPolicy&) = delete;
					IInteractivityPolicy(IInteractivityPolicy&&) = delete;

					virtual ~IInteractivityPolicy() = 0;

					IInteractivityPolicy& operator=(const IInteractivityPolicy&) = delete;
					IInteractivityPolicy& operator=(IInteractivityPolicy&&) = delete;

					const virtual bool MouseLDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) = 0;
					const virtual bool MouseRDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) = 0;
					const virtual bool MouseWheel(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point, int delta) = 0;
					const virtual bool MouseMove(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) = 0;
					const virtual bool MouseLUp(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) = 0;
				};
			}
		}
	}
}