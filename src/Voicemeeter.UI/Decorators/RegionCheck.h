#pragma once

#include <type_traits>
#include <utility>

#include "../IComponent.h"
#include "../Trackers/Input.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			template<typename TComponent>
			class RegionCheck : public TComponent {
				static_assert(
					::std::is_base_of_v<IComponent, TComponent>,
					"TComponent must be derived from IComponent");

			public:
				template<typename... Args>
				explicit RegionCheck(
					Trackers::Input& inputTracker,
					Args&&... args
				) : TComponent{ ::std::forward<Args>(args)... }
				  , m_inputTracker{ inputTracker } {

				}

				RegionCheck() = delete;
				RegionCheck(const RegionCheck&) = delete;
				RegionCheck(RegionCheck&&) = delete;

				~RegionCheck() = default;

				RegionCheck& operator=(const RegionCheck&) = delete;
				RegionCheck& operator=(RegionCheck&&) = delete;

				virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
					if (!is_overlapping(
							point, vertex,
							TComponent::get_Position(), TComponent::get_Size())) {
						return;
					}

					TComponent::Redraw(point, vertex);
				};
				virtual bool MouseLDown(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseLDown(point);
				};
				virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseLDouble(point);
				};
				virtual bool MouseMDown(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseMDown(point);
				};
				virtual bool MouseMDouble(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseMDouble(point);
				};
				virtual bool MouseRDown(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseRDown(point);
				};
				virtual bool MouseRDouble(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseRDouble(point);
				};
				virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseWheel(point, delta);
				};
				virtual bool MouseMove(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseMove(point);
				};
				virtual bool MouseLUp(const ::std::valarray<double>& point) override {
					return (IsInside(point) || m_inputTracker.get_Track(*this))
						&& TComponent::MouseLUp(point);
				};

			private:
				Trackers::Input& m_inputTracker;

				static inline bool is_inside(
					const ::std::valarray<double>& point,
					const ::std::valarray<double>& vertex
				) {
					return !(point.min() < 0)
						&& (point - vertex).max() < 0;
				};
				static inline bool is_overlapping(
					const ::std::valarray<double>& lhs_point,
					const ::std::valarray<double>& lhs_vertex,
					const ::std::valarray<double>& rhs_point,
					const ::std::valarray<double>& rhs_vertex
				) {
					return is_inside(rhs_point, lhs_point + lhs_vertex)
						&& is_inside(lhs_point, rhs_point + rhs_vertex);
				};
				inline bool IsInside(
					const ::std::valarray<double>& point
				) const {
					return is_inside(
						point - TComponent::get_Position(),
						TComponent::get_Size());
				};
			};
		}
	}
}