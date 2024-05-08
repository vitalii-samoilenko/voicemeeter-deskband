#pragma once

#include <unordered_map>
#include <vector>

#include "d2d1_2.h"
#include "dwrite.h"
#pragma comment(lib, "dwrite")
#include <atlbase.h>

#include "style.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class DrawingEngine {
					Style m_style;
					CComPtr<ID2D1Factory> m_pFactory;
					CComPtr<IDWriteFactory> m_pDwFactory;

				public:
					enum class resource_type {
						btn_round_normal,
						btn_round_active,
						bh_sub,
						bh_active
					};
					class Snapshot {
						friend class DrawingEngine;

						CComPtr<ID2D1HwndRenderTarget> m_pTarget;
						std::vector<CComPtr<ID2D1Resource>> m_resources;

					public:
						inline ID2D1HwndRenderTarget* get_pTarget() const noexcept {
							return m_pTarget;
						}
						template<typename T>
						inline const T* get_pPrimitive(size_t i) const {
							return m_primitives[i];
						}

						Snapshot() = default;
						Snapshot(const Snapshot&) = delete;
						Snapshot(Snapshot&&) = default;

						~Snapshot() = default;

						Snapshot& operator=(const Snapshot&) = delete;
						Snapshot& operator=(Snapshot&&) = default;
					};

					explicit DrawingEngine(
						Style style
					);
					DrawingEngine() = delete;
					DrawingEngine(const DrawingEngine&) = delete;
					DrawingEngine(DrawingEngine&&) = default;

					~DrawingEngine() = default;

					DrawingEngine& operator=(const DrawingEngine&) = delete;
					DrawingEngine& operator=(DrawingEngine&&) = default;

					Snapshot Initialize(
						HWND hWnd,
						const std::vector<resource_type>& resources
					);
				};
			}
		}
	}
}