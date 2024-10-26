#pragma once

#include <memory>

#include "estd/linear_algebra.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class RegionCheck final : public IComponent {
			public:
				explicit RegionCheck(
					::std::unique_ptr<IComponent> pComponent
				);
				RegionCheck(const RegionCheck&) = delete;
				RegionCheck(RegionCheck&&) = delete;

				~RegionCheck() = default;

				RegionCheck& operator=(const RegionCheck&) = delete;
				RegionCheck& operator=(RegionCheck&&) = delete;

				virtual const ::linear_algebra::vectord& get_Position() const override final;
				virtual const ::linear_algebra::vectord& get_Size() const override final;
				virtual const ::linear_algebra::vectord& get_BaseSize() const override final;

				virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override final;
				virtual void Rescale(const ::linear_algebra::vectord& vertex) override final;
				virtual void Move(const ::linear_algebra::vectord& point) override final;
				virtual bool MouseLDown(const ::linear_algebra::vectord& point) override final;
				virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override final;
				virtual bool MouseRDown(const ::linear_algebra::vectord& point) override final;
				virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override final;
				virtual bool MouseMove(const ::linear_algebra::vectord& point) override final;
				virtual bool MouseLUp(const ::linear_algebra::vectord& point) override final;

			private:
				::std::unique_ptr<IComponent> m_pComponent;
			};
		}
	}
}