#pragma once

#include <utility>

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "IInteractivityPolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TComponent, typename PointerFunc, typename WheelFunc>
				class PreconfiguredInteractivityPolicy final : public IInteractivityPolicy<TComponent> {
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
					) : m_onMouseLDown{ std::move(onMouseLDown) }
					  , m_onMouseRDown{ std::move(onMouseRDown) }
					  , m_onMouseWheel{ std::move(onMouseWheel) }
					  , m_onMouseMove{ std::move(onMouseMove) }
					  , m_onMouseLUp{ std::move(onMouseLUp) } {

					}
					PreconfiguredInteractivityPolicy() = delete;
					PreconfiguredInteractivityPolicy(const PreconfiguredInteractivityPolicy&) = delete;
					PreconfiguredInteractivityPolicy(PreconfiguredInteractivityPolicy&&) = delete;

					~PreconfiguredInteractivityPolicy() = default;

					PreconfiguredInteractivityPolicy& operator=(const PreconfiguredInteractivityPolicy&) = delete;
					PreconfiguredInteractivityPolicy& operator=(PreconfiguredInteractivityPolicy&&) = delete;

					virtual void MouseLDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const override {
						m_onMouseLDown(component, point);
					};
					virtual void MouseRDown(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const override {
						m_onMouseRDown(component, point);
					};
					virtual void MouseWheel(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point, int delta) const override {
						m_onMouseWheel(component, point, delta);
					};
					virtual void MouseMove(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const override {
						m_onMouseMove(component, point);
					};
					virtual void MouseLUp(estd::remove_cvref_t<TComponent>& component, linear_algebra::vector point) const override {
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