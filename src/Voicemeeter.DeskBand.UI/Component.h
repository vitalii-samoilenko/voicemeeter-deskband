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

				bool IsTrackingInput() const;
				void EnableInputTrack();
				void DisableInputTrack();

			protected:
				Component(
					IInputTracker& inputTracker,
					const ::linear_algebra::vectord& baseMarginTopLeft,
					const ::linear_algebra::vectord& baseMarginBottomRight
				);

				~Component() = default;

				virtual const ::linear_algebra::vectord& OnGet_Position() const = 0;
				virtual const ::linear_algebra::vectord& OnGet_Size() const = 0;
				virtual const ::linear_algebra::vectord& OnGet_BaseSize() const = 0;

				virtual void OnRedraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) = 0;
				virtual void OnRescale(const ::linear_algebra::vectord& vertex) = 0;
				virtual void OnMove(const ::linear_algebra::vectord& point) = 0;
				virtual void OnMouseLDown(const ::linear_algebra::vectord& point) = 0;
				virtual void OnMouseLDouble(const ::linear_algebra::vectord& point) = 0;
				virtual void OnMouseRDown(const ::linear_algebra::vectord& point) = 0;
				virtual void OnMouseWheel(const ::linear_algebra::vectord& point, int delta) = 0;
				virtual void OnMouseMove(const ::linear_algebra::vectord& point) = 0;
				virtual void OnMouseLUp(const ::linear_algebra::vectord& point) = 0;

			private:
				IInputTracker& m_inputTracker;
				::linear_algebra::vectord m_marginTopLeft;
				::linear_algebra::vectord m_marginBottomRight;
				::linear_algebra::vectord m_baseMarginTopLeft;
				::linear_algebra::vectord m_baseMarginBottomRight;
			};
		}
	}
}