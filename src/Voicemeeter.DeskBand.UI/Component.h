#pragma once

#include "estd/linear_algebra.h"

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Component : public IComponent {
			public:
				Component(const Component&) = delete;
				Component(Component&&) = delete;

				Component& operator=(const Component&) = delete;
				Component& operator=(Component&&) = delete;

				virtual linear_algebra::vector get_Position() const override final;
				virtual linear_algebra::vector get_Size() const override final;
				virtual linear_algebra::vector get_BaseSize() const override final;

				virtual void Redraw(linear_algebra::vector point, linear_algebra::vector vertex) override final;
				virtual void Rescale(linear_algebra::vector vertex) override final;
				virtual void Move(linear_algebra::vector point) override final;
				virtual bool MouseLDown(linear_algebra::vector point) override final;
				virtual bool MouseRDown(linear_algebra::vector point) override final;
				virtual bool MouseWheel(linear_algebra::vector point, int delta) override final;

			protected:
				linear_algebra::vector m_point;
				linear_algebra::vector m_vertex;
				linear_algebra::vector m_baseVertex;

				Component() = default;

				~Component() = default;

				virtual void OnRedraw(linear_algebra::vector point, linear_algebra::vector vertex) = 0;
				virtual void OnRescale(linear_algebra::vector vertex) = 0;
				virtual void OnMove(linear_algebra::vector point) = 0;
				virtual void OnMouseLDown(linear_algebra::vector point) = 0;
				virtual void OnMouseRDown(linear_algebra::vector point) = 0;
				virtual void OnMouseWheel(linear_algebra::vector point, int delta) = 0;
			};
		}
	}
}