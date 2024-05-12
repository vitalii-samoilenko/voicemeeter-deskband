#pragma once

#include <windows.h>

#include "drawingengine.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Scene {
				public:
					inline explicit Scene(DrawingEngine& drwEngine) noexcept
						: m_drwEngine{ drwEngine }
						, m_pCtx{ nullptr } {

					}
					Scene() = delete;
					Scene(const Scene&) = delete;
					Scene(Scene&&) = delete;

					~Scene() = default;

					Scene& operator=(const Scene&) = delete;
					Scene& operator=(Scene&&) = delete;

					void Initialize(HWND hWnd);
					void Resize(UINT w, UINT h) const;
					void Draw() const;

				private:
					DrawingEngine& m_drwEngine;
					std::unique_ptr<DrawingEngine::Context> m_pCtx;
				};
			}
		}
	}
}