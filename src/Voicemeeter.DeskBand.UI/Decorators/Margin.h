#pragma once

#include <memory>

#include "estd/linear_algebra.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Margin final : public IComponent {
			public:
				Margin(
					::std::unique_ptr<IComponent> pComponent,
					const ::linear_algebra::vectord& baseMarginTopLeft,
					const ::linear_algebra::vectord& baseMarginBottomRight
				);
				Margin(const Margin&) = delete;
				Margin(Margin&&) = delete;

				~Margin() = default;

				Margin& operator=(const Margin&) = delete;
				Margin& operator=(Margin&&) = delete;

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
				::linear_algebra::vectord m_position;
				::linear_algebra::vectord m_vertex;
				::linear_algebra::vectord m_baseVertex;
				::linear_algebra::vectord m_marginTopLeft;
				::linear_algebra::vectord m_marginBottomRight;
				::linear_algebra::vectord m_baseMarginTopLeft;
				::linear_algebra::vectord m_baseMarginBottomRight;
			};
		}
	}
}