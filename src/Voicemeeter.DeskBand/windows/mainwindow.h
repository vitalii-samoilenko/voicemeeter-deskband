#pragma once

#include "panel.h"
#include "error.h"
#include "../messages.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class MainWindow : public Window {
				static std::once_flag WndMainClassGuard;

				std::unique_ptr<Panel> m_pPanel;

			protected:
				virtual LRESULT OnSize(UINT w, UINT h) override;
				virtual LRESULT OnDestroy() override;

			public:
				explicit MainWindow(
					HINSTANCE hInstance
				);
				MainWindow() = delete;
				MainWindow(const MainWindow&) = delete;
				MainWindow(MainWindow&&) = delete;

				~MainWindow() = default;

				MainWindow& operator=(const MainWindow&) = delete;
				MainWindow& operator=(MainWindow&&) = delete;

				virtual void Initialize() override;
				virtual void Show(int nCmdShow) override;

				template<typename T, typename ...Args>
				T* MakePanel(Args... args) {
					if (m_pPanel) {
						throw windows_error{ MSG_ERR_GENERAL, "Panel is already made" };
					}

					m_pPanel.reset(new T{ *this, std::forward(args)... });

					return reinterpret_cast<T*>(m_pPanel.get());
				}
			};
		}
	}
}