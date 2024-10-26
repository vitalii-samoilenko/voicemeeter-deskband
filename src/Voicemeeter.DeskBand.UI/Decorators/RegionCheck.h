#pragma once

#include <type_traits>

#include "estd/linear_algebra.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Decorators {
				template<typename TComponent>
				class RegionCheck : public TComponent {
					static_assert(
						::std::is_base_of_v<IComponent, TComponent>,
						"TComponent must be derived from IComponent");

				public:
					using TComponent::TComponent;

					RegionCheck() = delete;
					RegionCheck(const RegionCheck&) = delete;
					RegionCheck(RegionCheck&&) = delete;

					~RegionCheck() = default;

					RegionCheck& operator=(const RegionCheck&) = delete;
					RegionCheck& operator=(RegionCheck&&) = delete;

					virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
						if (!::linear_algebra::is_overlapping(
								point, vertex,
								TComponent::get_Position(), TComponent::get_Size())) {
							return;
						}

						TComponent::Redraw(point, vertex);
					};
					virtual bool MouseLDown(const ::linear_algebra::vectord& point) override {
						return is_inside(point)
							&& TComponent::MouseLDown(point);
					};
					virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override {
						return is_inside(point)
							&& TComponent::MouseLDouble(point);
					};
					virtual bool MouseRDown(const ::linear_algebra::vectord& point) override {
						return is_inside(point)
							&& TComponent::MouseRDown(point);
					};
					virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override {
						return is_inside(point)
							&& TComponent::MouseWheel(point, delta);
					};
					virtual bool MouseMove(const ::linear_algebra::vectord& point) override {
						return is_inside(point)
							&& TComponent::MouseMove(point);
					};
					virtual bool MouseLUp(const ::linear_algebra::vectord& point) override {
						return is_inside(point)
							&& TComponent::MouseLUp(point);
					};

				private:
					inline bool is_inside(const ::linear_algebra::vectord& point) {
						return ::linear_algebra::is_inside(
							point - TComponent::get_Position(),
							TComponent::get_Size());
					}
				};
			}
		}
	}
}