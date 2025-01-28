#pragma once

#include "Voicemeeter.UI.Cherry/Graphics/Glyph.h"

#include "Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace D3D12 {
			namespace Graphics {
				template<typename TBundle>
				using Glyph = Cherry::Graphics::Glyph<
					Instrumentation,
					TBundle>;
			}
		}
	}
}