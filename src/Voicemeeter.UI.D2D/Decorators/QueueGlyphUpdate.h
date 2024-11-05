#pragma once

#include <type_traits>

#include "Voicemeeter.UI/Policies/IGlyphUpdate.h"

#include "../Graphics/Canvas.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Decorators {
				template<typename TGlyph, typename TState, typename TGlyphUpdate>
				class QueueGlyphUpdate : public TGlyphUpdate {
					static_assert(
						::std::is_base_of_v<Policies::IGlyphUpdate<TGlyph, TState>, TGlyphUpdate>,
						"TGlyphUpdate must be derived from IGlyphUpdate<TGlyph, TState>");

				public:
					explicit QueueGlyphUpdate(
						const Graphics::Canvas& canvas
					) : m_canvas{ canvas } {

					}
					QueueGlyphUpdate() = delete;
					QueueGlyphUpdate(const QueueGlyphUpdate&) = delete;
					QueueGlyphUpdate(QueueGlyphUpdate&&) = delete;

					~QueueGlyphUpdate() = default;

					QueueGlyphUpdate& operator=(const QueueGlyphUpdate&) = delete;
					QueueGlyphUpdate& operator=(QueueGlyphUpdate&&) = delete;

					virtual void Update(TGlyph& glyph, const TState& state) const {
						TGlyphUpdate::Update(glyph, state);

						m_canvas.get_Queue()
							.Push(glyph);
					};

				private:
					const Graphics::Canvas& m_canvas;
				};
			}
		}
	}
}