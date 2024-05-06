#pragma once

#include "drawingengine.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Scene {

			protected:
				Scene() = default;

			public:
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				virtual ~Scene() = 0 { };

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				virtual void Resize(const Palette& rPalette) = 0 { };
			};
		}
	}
}