#pragma once

#include "estd/linear_algebra.h"

#include "IAppInputTracker.h"
#include "IComponent.h"
#include "IInputTracker.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			class InputTracker final : public IInputTracker {
			public:
				explicit InputTracker(
					IAppInputTracker& appInputTracler
				);
				InputTracker(const InputTracker&) = delete;
				InputTracker(InputTracker&&) = delete;

				~InputTracker() = default;

				InputTracker& operator=(const InputTracker&) = delete;
				InputTracker& operator=(InputTracker&&) = delete;

				virtual bool IsTracking(const IComponent& component) const override;

				virtual void EnableInputTrack(IComponent& component) override;
				virtual void DisableInputTrack(IComponent& component) override;
				virtual bool MouseLDown(const ::linear_algebra::vectord& point) override;
				virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override;
				virtual bool MouseRDown(const ::linear_algebra::vectord& point) override;
				virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override;
				virtual bool MouseMove(const ::linear_algebra::vectord& point) override;
				virtual bool MouseLUp(const ::linear_algebra::vectord& point) override;

			private:
				IAppInputTracker& m_appInputTracler;
				IComponent* m_pPinned;
			};
		}
	}
}