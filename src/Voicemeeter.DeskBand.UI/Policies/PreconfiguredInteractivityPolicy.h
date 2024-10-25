#pragma once

#include <utility>

#include "estd/linear_algebra.h"
#include "estd/type_traits.h"

#include "IInteractivityPolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TComponent,
					typename TOnMouseLDown,
					typename TOnMouseLDouble,
					typename TOnMouseRDown,
					typename TOnMouseWheel,
					typename TOnMouseMove, 
					typename TOnMouseLUp>
				class PreconfiguredInteractivityPolicy final : public IInteractivityPolicy<TComponent> {
					static_assert(
						::estd::is_invocable_r<void, TOnMouseLDown, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&>(),
						"TOnMouseLDown must be invocable with TComponent& and const vector& arguments and void return type");
					static_assert(
						::estd::is_invocable_r<void, TOnMouseLDouble, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&>(),
						"TOnMouseLDouble must be invocable with TComponent& and const vector& arguments and void return type");
					static_assert(
						::estd::is_invocable_r<void, TOnMouseRDown, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&>(),
						"TOnMouseRDown must be invocable with TComponent& and const vector& arguments and void return type");
					static_assert(
						::estd::is_invocable_r<void, TOnMouseWheel, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&, int>(),
						"TOnMouseWheel must be invocable with TComponent&, const vector& and int arguments and void return type");
					static_assert(
						::estd::is_invocable_r<void, TOnMouseMove, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&>(),
						"TOnMouseMove must be invocable with TComponent& and const vector& arguments and void return type");
					static_assert(
						::estd::is_invocable_r<void, TOnMouseLUp, ::estd::remove_cvref_t<TComponent>&, const ::linear_algebra::vector&>(),
						"TOnMouseLUp must be invocable with TComponent& and const vector& arguments and void return type");

				public:
					PreconfiguredInteractivityPolicy(
						TOnMouseLDown onMouseLDown,
						TOnMouseLDouble onMouseLDouble,
						TOnMouseRDown onMouseRDown,
						TOnMouseWheel onMouseWheel,
						TOnMouseMove onMouseMove,
						TOnMouseLUp onMouseLUp
					) : m_onMouseLDown{ ::std::move(onMouseLDown) }
					  , m_onMouseLDouble{ ::std::move(onMouseLDouble) }
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
					virtual void MouseLDouble(::estd::remove_cvref_t<TComponent>& component, const ::linear_algebra::vector& point) const override {
						m_onMouseLDouble(component, point);
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
					TOnMouseLDown m_onMouseLDown;
					TOnMouseLDouble m_onMouseLDouble;
					TOnMouseRDown m_onMouseRDown;
					TOnMouseWheel m_onMouseWheel;
					TOnMouseMove m_onMouseMove;
					TOnMouseLUp m_onMouseLUp;
				};
			}
		}
	}
}