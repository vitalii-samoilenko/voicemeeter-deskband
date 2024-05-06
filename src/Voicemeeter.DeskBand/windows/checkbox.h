#pragma once

#include "control.h"
#include "checkboxscene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Checkbox : public Control {
				static std::once_flag CtlCheckboxClassGuard;

				bool m_checked;
				CheckboxScene m_scene;

			protected:
				inline virtual CheckboxScene& get_rScene() noexcept override {
					return m_scene;
				};

				virtual LRESULT OnPaint() override;
				virtual LRESULT OnLButtonDown(int x, int y) override;

			public:
				Checkbox(
					Panel& rPnlParent,
					DrawingEngine& rDrwEngine,
					LPCWSTR lpszLabel
				);
				Checkbox() = delete;
				Checkbox(const Checkbox&) = delete;
				Checkbox(Checkbox&&) = delete;

				~Checkbox() = default;

				Checkbox& operator=(const Checkbox&) = delete;
				Checkbox& operator=(Checkbox&&) = delete;
			};
		}
	}
}