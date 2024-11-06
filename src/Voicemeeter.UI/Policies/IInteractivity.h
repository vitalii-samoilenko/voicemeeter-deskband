#pragma once

#include <type_traits>

#include "estd/linear_algebra.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TComponent>
			class IInteractivity {
				static_assert(
					::std::is_base_of_v<IComponent, TComponent>,
					"TComponent must be derived from IComponent");

			public:
				IInteractivity(const IInteractivity&) = delete;
				IInteractivity(IInteractivity&&) = delete;

				virtual ~IInteractivity() = default;

				IInteractivity& operator=(const IInteractivity&) = delete;
				IInteractivity& operator=(IInteractivity&&) = delete;

				virtual void set_Focus(TComponent& component, bool value) const = 0;

				virtual void MouseLDown(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseLDouble(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseMDown(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseMDouble(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseRDown(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseRDouble(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseWheel(TComponent& component, const ::linear_algebra::vectord& point, int delta) const = 0;
				virtual void MouseMove(TComponent& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseLUp(TComponent& component, const ::linear_algebra::vectord& point) const = 0;

			protected:
				IInteractivity() = default;
			};
		}
	}
}