#pragma once

#include "Voicemeeter.UI/Scene.h"

#include "Graphics/Canvas.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			class Scene : public UI::Scene<Graphics::Canvas> {
			public:
				using UI::Scene<Graphics::Canvas>::Scene;

				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
				void Resize(const ::std::valarray<double>& vertex) override;
			};
		}
	}
}