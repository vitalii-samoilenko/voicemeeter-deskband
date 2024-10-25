#pragma once

#include "estd/linear_algebra.h"

#include "IInputTracker.h"

#include "IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Component : public IComponent {
			public:
				Component() = delete;
				Component(const Component&) = delete;
				Component(Component&&) = delete;

				Component& operator=(const Component&) = delete;
				Component& operator=(Component&&) = delete;

				virtual const ::linear_algebra::vector& get_Position() const override final;
				virtual const ::linear_algebra::vector& get_Size() const override final;
				virtual const ::linear_algebra::vector& get_BaseSize() const override final;

				virtual void Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override final;
				virtual void Rescale(const ::linear_algebra::vector& vertex) override final;
				virtual void Move(const ::linear_algebra::vector& point) override final;
				virtual bool MouseLDown(const ::linear_algebra::vector& point) override final;
				virtual bool MouseLDouble(const ::linear_algebra::vector& point) override final;
				virtual bool MouseRDown(const ::linear_algebra::vector& point) override final;
				virtual bool MouseWheel(const ::linear_algebra::vector& point, int delta) override final;
				virtual bool MouseMove(const ::linear_algebra::vector& point) override final;
				virtual bool MouseLUp(const ::linear_algebra::vector& point) override final;

				bool IsTrackingInput() const;
				void EnableInputTrack();
				void DisableInputTrack();

			protected:
				Component(
					IInputTracker& inputTracker,
					const ::linear_algebra::vector& baseMarginTopLeft,
					const ::linear_algebra::vector& baseMarginBottomRight
				);

				~Component() = default;

				virtual const ::linear_algebra::vector& OnGet_Position() const = 0;
				virtual const ::linear_algebra::vector& OnGet_Size() const = 0;
				virtual const ::linear_algebra::vector& OnGet_BaseSize() const = 0;

				virtual void OnRedraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) = 0;
				virtual void OnRescale(const ::linear_algebra::vector& vertex) = 0;
				virtual void OnMove(const ::linear_algebra::vector& point) = 0;
				virtual void OnMouseLDown(const ::linear_algebra::vector& point) = 0;
				virtual void OnMouseLDouble(const ::linear_algebra::vector& point) = 0;
				virtual void OnMouseRDown(const ::linear_algebra::vector& point) = 0;
				virtual void OnMouseWheel(const ::linear_algebra::vector& point, int delta) = 0;
				virtual void OnMouseMove(const ::linear_algebra::vector& point) = 0;
				virtual void OnMouseLUp(const ::linear_algebra::vector& point) = 0;

			private:
				IInputTracker& m_inputTracker;
				::linear_algebra::vector m_marginTopLeft;
				::linear_algebra::vector m_marginBottomRight;
				::linear_algebra::vector m_baseMarginTopLeft;
				::linear_algebra::vector m_baseMarginBottomRight;
			};
		}
	}
}