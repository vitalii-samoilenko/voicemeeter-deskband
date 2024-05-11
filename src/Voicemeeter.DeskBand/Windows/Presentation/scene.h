#pragma once

#include <vector>
#include <windows.h>

#include "drawingengine.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Scene {
				public:
					inline explicit Scene(DrawingEngine& drwEngine)
						: m_drwEngine{ drwEngine }
						, m_ctx{ DrawingEngine::Context::Empty() }
						, m_cLbl{}
						, m_cFrame{}
						, m_cBrush{}
						, m_scale{D2D1::IdentityMatrix()} {

					}
					Scene() = delete;
					Scene(const Scene&) = delete;
					Scene(Scene&&) = delete;

					~Scene() = default;

					Scene& operator=(const Scene&) = delete;
					Scene& operator=(Scene&&) = delete;

					void Initialize(HWND hWnd);
					void Resize(UINT w, UINT h);
					void Draw();

				private:
					DrawingEngine& m_drwEngine;
					DrawingEngine::Context m_ctx;
					std::vector<DrawingEngine::Context::Text> m_cLbl;
					std::vector<DrawingEngine::Context::Glyph> m_cFrame;
					std::vector<DrawingEngine::Context::Brush> m_cBrush;
					D2D1_MATRIX_3X2_F m_scale;
				};
			}
		}
	}
}