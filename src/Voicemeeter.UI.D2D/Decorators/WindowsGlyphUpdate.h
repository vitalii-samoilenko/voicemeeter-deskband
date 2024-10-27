#pragma once

#include <cmath>
#include <type_traits>

#include "estd/linear_algebra.h"

#include "Windows/Wrappers.h"

#include "Voicemeeter.UI/Graphics/IGlyph.h"
#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"

namespace Voicemeeter {
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
							static_cast<LONG>(::std::floor(topLeft.x)), static_cast<LONG>(::std::floor(topLeft.y)),
							static_cast<LONG>(::std::ceil(bottomRight.x)), static_cast<LONG>(::std::ceil(bottomRight.y)),
						};

						::Windows::wInvalidateRect(m_hWnd, &rc, TRUE);
					};

				private:
					HWND m_hWnd;
				};
			}
		}
	}
}