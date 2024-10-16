#pragma once

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "IInteractivityPolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TComponent, typename PointerFunc, typename WheelFunc>
				class PreconfiguredInteractivityPolicy : public IInteractivityPolicy<TComponent> {
					static_assert(estd::is_invocable<void, PointerFunc, estd::remove_cvref_t<TComponent>&, linear_algebra::vector>(),
						"PointerFunc must be invocable with TComponent and vector arguments and void return type");
					static_assert(estd::is_invocable<void, WheelFunc, estd::remove_cvref_t<TComponent>&, linear_algebra::vector, int>(),
						"WheelFunc must be invocable with TComponent, vector and int arguments and void return type");

				public:
					PreconfiguredInteractivityPolicy(
						PointerFunc onMouseLDown,
						PointerFunc onMouseRDown,
						WheelFunc onMouseWheel,
						PointerFunc onMouseMove,
						PointerFunc onMouseLUp
					) : m_onMouseLDown{ onMouseLDown }
					  , m_onMouseRDown{ onMouseRDown }
					  , m_onMouseWheel{ onMouseWheel }
					  , m_onMouseMove{ onMouseMove }
					  , m_onMouseLUp{ onMouseLUp } {

					}
					PreconfiguredInteractivityPolicy() = delete;
					PreconfiguredInteractivityPolicy(const PreconfiguredInteractivityPolicy&) = delete;
					PreconfiguredInteractivityPolicy(PreconfiguredInteractivityPolicy&&) = delete;

					~PreconfiguredInteractivityPolicy() = default;

					PreconfiguredInteractivityPolicy& operator=(const PreconfiguredInteractivityPolicy&) = delete;
					PreconfiguredInteractivityPolicy& operator=(PreconfiguredInteractivityPolicy&&) = delete;

					const virtual void MouseLDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) override {
						m_onMouseLDown(component, point);
					};
					const virtual void MouseRDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) override {
						m_onMouseRDown(component, point);
					};
					const virtual void MouseWheel(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point, int delta) override {
						m_onMouseWheel(component, point, delta);
					};
					const virtual void MouseMove(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) override {
						m_onMouseMove(component, point);
					};
					const virtual void MouseLUp(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) override {
						m_onMouseLUp(component, point);
					};

				private:
					PointerFunc m_onMouseLDown;
					PointerFunc m_onMouseRDown;
					WheelFunc m_onMouseWheel;
					PointerFunc m_onMouseMove;
					PointerFunc m_onMouseLUp;
				};
			}
		}
	}
}