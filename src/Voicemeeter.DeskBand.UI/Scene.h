#pragma once

#include <memory>

#include "estd/linear_algebra.h"

#include "IComponent.h"
#include "IInputTracker.h"
#include "IScene.h"
#include "Graphics/ICanvas.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Scene final : public IScene {
			public:
				Scene(
					::std::unique_ptr<IInputTracker> inputTracker,
					::std::unique_ptr<ICanvas> pCanvas,
					::std::unique_ptr<IComponent> pComposition
				);
				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				virtual const ::linear_algebra::vectord& get_Position() const override;
				virtual const ::linear_algebra::vectord& get_Size() const override;

				virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;
				virtual void Resize(const ::linear_algebra::vectord& vertex) override;
				virtual bool MouseLDown(const ::linear_algebra::vectord& point) override;
				virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override;
				virtual bool MouseRDown(const ::linear_algebra::vectord& point) override;
				virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override;
				virtual bool MouseMove(const ::linear_algebra::vectord& point) override;
				virtual bool MouseLUp(const ::linear_algebra::vectord& point) override;

			private:
				::std::unique_ptr<IInputTracker> m_inputTracker;
				::std::unique_ptr<ICanvas> m_pCanvas;
				::std::unique_ptr<IComponent> m_pComposition;
			};
		}
	}
}