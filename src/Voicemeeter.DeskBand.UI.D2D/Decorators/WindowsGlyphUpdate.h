#pragma once

#include <type_traits>

#include "estd/linear_algebra.h"

#include <windows.h>

#include "Voicemeeter.DeskBand.UI/Graphics/IGlyph.h"
#include "Voicemeeter.DeskBand.UI/Policies/IGlyphUpdate.h"
#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

using namespace ::Voicemeeter::DeskBand::Windows;

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace D2D {
				namespace Decorators {
					template<typename TGlyph, typename TState, typename TGlyphUpdate>
					class WindowsGlyphUpdate : public TGlyphUpdate {
						static_assert(
							::std::is_base_of_v<UI::Graphics::IGlyph, TGlyph>,
							"TGlyph must be derived from Glyph");
						static_assert(
							::std::is_base_of_v<Policies::IGlyphUpdate<TGlyph, TState>, TGlyphUpdate>,
							"TGlyphUpdate must be derived from IGlyphUpdate<TGlyph, TState>");

					public:
						explicit WindowsGlyphUpdate(
							HWND hWnd
						) : m_hWnd{ hWnd } {

						}
						WindowsGlyphUpdate() = delete;
						WindowsGlyphUpdate(const WindowsGlyphUpdate&) = delete;
						WindowsGlyphUpdate(WindowsGlyphUpdate&&) = delete;

						~WindowsGlyphUpdate() = default;

						WindowsGlyphUpdate& operator=(const WindowsGlyphUpdate&) = delete;
						WindowsGlyphUpdate& operator=(WindowsGlyphUpdate&&) = delete;

						virtual void Update(TGlyph& glyph, const TState& state) const {
							TGlyphUpdate::Update(glyph, state);

							::linear_algebra::vectord topLeft{ glyph.get_Position() };
							::linear_algebra::vectord bottomRight{ topLeft + glyph.get_Size() };
							RECT rc{
								static_cast<LONG>(topLeft.x), static_cast<LONG>(topLeft.y),
								static_cast<LONG>(bottomRight.x), static_cast<LONG>(bottomRight.y),
							};

							wInvalidateRect(m_hWnd, &rc, TRUE);
						};

					private:
						HWND m_hWnd;
					};
				}
			}
		}
	}
}