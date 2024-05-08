#pragma once

#include "../window.h"
#include "drawingengine.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Scene {
					Window& m_rWnd;
					DrawingEngine& m_rDrwEngine;
					const std::vector<DrawingEngine::resource_type>& m_resources;
					DrawingEngine::Snapshot m_snapshot;

				protected:
					Scene(
						Window& rWnd,
						DrawingEngine& rDrwEngine,
						const std::vector<DrawingEngine::resource_type>& resources
					);

				public:
					Scene(const Scene&) = delete;
					Scene(Scene&&) = delete;

					virtual ~Scene() = 0 { };

					Scene& operator=(const Scene&) = delete;
					Scene& operator=(Scene&&) = delete;

					virtual void Initialize();
					virtual void Resize(UINT w, UINT h);
					virtual bool ContainsPoint(LONG x, LONG y) = 0 { };
					virtual HRESULT Draw() = 0 { };
				};
			}
		}
	}
}