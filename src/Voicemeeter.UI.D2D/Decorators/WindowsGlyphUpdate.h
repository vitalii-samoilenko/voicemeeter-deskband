#pragma once

#include <cmath>
#include <type_traits>

#include "Windows/Wrappers.h"

#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				template<typename TGlyph, typename TState, typename TGlyphUpdate>
				class WindowsGlyphUpdate : public TGlyphUpdate {
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

						::std::valarray<double> topLeft{ glyph.get_Position() };
						::std::valarray<double> bottomRight{ topLeft + glyph.get_Size() };
						RECT rc{
							static_cast<LONG>(::std::floor(topLeft[0])),
							static_cast<LONG>(::std::floor(topLeft[1])),
							static_cast<LONG>(::std::ceil(bottomRight[0])),
							static_cast<LONG>(::std::ceil(bottomRight[1])),
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