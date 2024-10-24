#pragma once

#include <utility>

#include "estd/linear_algebra.h"
#include "estd/type_traits.h"

#include "IInteractivityPolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TComponent, typename TPointerFunc, typename TWheelFunc>
				class PreconfiguredInteractivityPolicy final : public IInteractivityPolicy<TComponent> {
					static_assert(
						::estd::is_invocable_r<void, TPointerFunc, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&>(),
						"TPointerFunc must be invocable with TComponent& and const vector& arguments and void return type");
					static_assert(
						::estd::is_invocable_r<void, TWheelFunc, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&, int>(),
						"TWheelFunc must be invocable with TComponent&, const vector& and int arguments and void return type");

				public:
					PreconfiguredInteractivityPolicy(
						TPointerFunc onMouseLDown,
						TPointerFunc onMouseRDown,
						TWheelFunc onMouseWheel,
						TPointerFunc onMouseMove,
						TPointerFunc onMouseLUp
					) : m_onMouseLDown{ ::std::move(onMouseLDown) }
					  , m_onMouseRDown{ ::std::move(onMouseRDown) }
					  , m_onMouseWheel{ ::std::move(onMouseWheel) }
					  , m_onMouseMove{ ::std::move(onMouseMove) }
					  , m_onMouseLUp{ ::std::move(onMouseLUp) } {

					}
					PreconfiguredInteractivityPolicy() = delete;
					PreconfiguredInteractivityPolicy(const PreconfiguredInteractivityPolicy&) = delete;
					PreconfiguredInteractivityPolicy(PreconfiguredInteractivityPolicy&&) = delete;

					~PreconfiguredInteractivityPolicy() = default;

					PreconfiguredInteractivityPolicy& operator=(const PreconfiguredInteractivityPolicy&) = delete;
					PreconfiguredInteractivityPolicy& operator=(PreconfiguredInteractivityPolicy&&) = delete;

					virtual void MouseLDown(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vector& point) const override {
						m_onMouseLDown(component, point);
					};
					virtual void MouseRDown(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vector& point) const override {
						m_onMouseRDown(component, point);
					};
					virtual void MouseWheel(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vector& point, int delta) const override {
						m_onMouseWheel(component, point, delta);
					};
					virtual void MouseMove(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vector& point) const override {
						m_onMouseMove(component, point);
					};
					virtual void MouseLUp(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vector& point) const override {
						m_onMouseLUp(component, point);
					};

				private:
					TPointerFunc m_onMouseLDown;
					TPointerFunc m_onMouseRDown;
					TWheelFunc m_onMouseWheel;
					TPointerFunc m_onMouseMove;
					TPointerFunc m_onMouseLUp;
				};
			}
		}
	}
}