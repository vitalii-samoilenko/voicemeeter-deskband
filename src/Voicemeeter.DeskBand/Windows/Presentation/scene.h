#pragma once

#include <vector>

#include "../window.h"
#include "resource.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Scene {
					Window& m_rWnd;
					DrawingEngine& m_rDrwEngine;
					std::vector<Brush> m_cBrush;
					std::vector<Glyph> m_cGlyph;
					std::unique_ptr<Frame> m_pFrame;

				protected:
					Scene(
						Window& rWnd,
						DrawingEngine& rDrwEngine
					);

				public:
					Scene(const Scene&) = delete;
					Scene(Scene&&) = delete;

					virtual ~Scene() = 0 { };

					Scene& operator=(const Scene&) = delete;
					Scene& operator=(Scene&&) = delete;

					virtual void Initialize();
					virtual void Resize(UINT w, UINT h);
					bool ContainsPoint(LONG x, LONG y);
					virtual void Draw() = 0 { };
				};
			}
		}
	}
}