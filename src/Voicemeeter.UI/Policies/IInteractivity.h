#pragma once

#include <type_traits>

#include "estd/linear_algebra.h"
#include "estd/type_traits.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TComponent>
			class IInteractivity {
				static_assert(
					::std::is_base_of_v<IComponent, ::estd::remove_cvref_t<TComponent>>,
					"TComponent must be derived from IComponent");

			public:
				IInteractivity(const IInteractivity&) = delete;
				IInteractivity(IInteractivity&&) = delete;

				virtual ~IInteractivity() = default;

				IInteractivity& operator=(const IInteractivity&) = delete;
				IInteractivity& operator=(IInteractivity&&) = delete;

				virtual void set_Focus(::estd::remove_cvref_t<TComponent>& component, bool value) const = 0;

				virtual void MouseLDown(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseLDouble(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseMDown(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseMDouble(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseRDown(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseRDouble(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseWheel(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point, int delta) const = 0;
				virtual void MouseMove(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;
				virtual void MouseLUp(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vectord& point) const = 0;

			protected:
				IInteractivity() = default;
			};
		}
	}
}