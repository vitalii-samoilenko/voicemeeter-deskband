#pragma once

#include "scene.h"
#include "../statecontrol.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class StateScene : public Scene {
					StateControl& m_rCtrlState;

				public:
					explicit StateScene(
						StateControl& rCtrlState
					);
					StateScene() = delete;
					StateScene(const StateScene&) = delete;
					StateScene(StateScene&&) = delete;

					~StateScene() = default;

					StateScene& operator=(const StateScene&) = delete;
					StateScene& operator=(StateScene&&) = delete;

					virtual void Initialize() override;
					virtual void Resize(UINT w, UINT h) override;
					virtual bool ContainsPoint(LONG x, LONG y) override;
					virtual HRESULT Draw() override;
				};
			}
		}
	}
}