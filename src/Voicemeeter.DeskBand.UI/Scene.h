#pragma once

#include <memory>

#include "estd/linear_algebra.h"

#include "IScene.h"
#include "IMouseTracker.h"
#include "IAppMouseTracker.h"
#include "Graphics/ICanvas.h"
#include "IComponent.h"

using namespace ::Voicemeeter::DeskBand::UI::Graphics;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class Scene final : public IScene, public IMouseTracker {
			public:
				Scene(
					IAppMouseTracker& appMouseTracker,
					::std::unique_ptr<IComponent> pComposition,
					::std::unique_ptr<ICanvas> pCanvas
				);
				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				virtual void Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override;
				virtual void Resize(const ::linear_algebra::vector& vertex) override;
				virtual bool MouseLDown(const ::linear_algebra::vector& point) override;
				virtual bool MouseRDown(const ::linear_algebra::vector& point) override;
				virtual bool MouseWheel(const ::linear_algebra::vector& point, int delta) override;
				virtual void MouseMove(const ::linear_algebra::vector& point) override;
				virtual void MouseLUp(const ::linear_algebra::vector& point) override;
				virtual void EnableMouseTrack(IComponent& pComponent) override;
				virtual void DisableMouseTrack(IComponent& pComponent) override;

			private:
				IAppMouseTracker& m_appMouseTracker;
				::std::unique_ptr<IComponent> m_pComposition;
				::std::unique_ptr<ICanvas> m_pCanvas;
				IComponent* m_pPinned;
			};
		}
	}
}