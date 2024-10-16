#pragma once

#include <memory>

#include "estd/linear_algebra.h"

#include "IScene.h"
#include "IMouseTracker.h"
#include "IAppMouseTracker.h"
#include "Graphics/ICanvas.h"
#include "IComponent.h"

using namespace Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Scene : public IScene, public IMouseTracker {
			public:
				Scene(
					IAppMouseTracker* pAppMouseTracker,
					IComponent* pComposition,
					ICanvas* pCanvas
				);
				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				const virtual linear_algebra::vector get_Position() override;
				const virtual linear_algebra::vector get_Size() override;

				virtual void Redraw(linear_algebra::vector region) override;
				virtual void Resize(linear_algebra::vector size) override;
				virtual bool MouseLDown(linear_algebra::vector point) override;
				virtual bool MouseRDown(linear_algebra::vector point) override;
				virtual bool MouseWheel(linear_algebra::vector point, int delta) override;
				virtual void MouseMove(linear_algebra::vector point) override;
				virtual void MouseLUp(linear_algebra::vector point) override;
				virtual void EnableMouseTrack(IComponent* pComponent) override;
				virtual void DisableMouseTrack(IComponent* pComponent) override;

			private:
				IAppMouseTracker* m_pAppMouseTracker;
				std::unique_ptr<IComponent> m_pComposition;
				std::unique_ptr<ICanvas> m_pCanvas;
				IComponent* m_pPinned;
			};
		}
	}
}