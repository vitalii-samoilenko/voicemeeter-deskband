#pragma once

#include "scene.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Panel;

			class Control : public Window {
				Panel& m_rPnlParent;
				DrawingEngine& m_rDrwEngine;

			protected:
				inline DrawingEngine& get_rDrwEngine() const noexcept {
					return m_rDrwEngine;
				};
				virtual Scene& get_rScene() noexcept = 0 { };

				virtual LRESULT OnCreate() override;
				virtual LRESULT OnSize() override;

				Control(
					std::once_flag& rWndClassGuard,
					LPCWSTR lpszClassName,
					Panel& rPnlParent,
					DrawingEngine& rDrwEngine
				);

			public:
				Control() = delete;
				Control(const Control&) = delete;
				Control(Control&&) = delete;

				~Control() = 0 { };

				Control& operator=(const Control&) = delete;
				Control& operator=(Control&&) = delete;

				virtual void Initialize() override;
			};
		}
	}
}