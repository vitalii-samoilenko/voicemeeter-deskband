#pragma once

#include "window.h"
#include "Presentation/scene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Panel;

			class Control : public Window {
				Panel& m_rPnlParent;

			protected:
				virtual Presentation::Scene& get_rScene() noexcept = 0 { };

				virtual LRESULT OnCreate() override;
				virtual LRESULT OnSize(UINT w, UINT h) override;
				virtual LRESULT OnPaint() override;

				Control(
					std::once_flag& rWndClassGuard,
					LPCWSTR lpszClassName,
					Panel& rPnlParent
				);

			public:
				Control() = delete;
				Control(const Control&) = delete;
				Control(Control&&) = delete;

				~Control() = default;

				Control& operator=(const Control&) = delete;
				Control& operator=(Control&&) = delete;

				virtual void Initialize() override;
			};
		}
	}
}