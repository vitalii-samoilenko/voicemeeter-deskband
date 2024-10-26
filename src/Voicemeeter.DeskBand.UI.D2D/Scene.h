#pragma once

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.Windows/Wrappers.h"

#include "Voicemeeter.DeskBand.UI/Scene.h"

#include "Graphics/Canvas.h"

using namespace ::Voicemeeter::DeskBand::Windows;

namespace Voicemeeter {
	namespace DeskBand {
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

					void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override;
				};
			}
		}
	}
}