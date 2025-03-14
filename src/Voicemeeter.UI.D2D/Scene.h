#pragma once

#include "Graphics/Canvas.h"
#include "Voicemeeter.UI/Scene.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			class Scene final : public UI::Scene {
			public:
				Scene(
					::std::unique_ptr<Graphics::Palette>& pPalette,
					::std::unique_ptr<UI::Trackers::Input>& pInputTracker,
					::std::unique_ptr<UI::Trackers::Focus>& pFocusTracker,
					::std::unique_ptr<UI::Graphics::ICanvas>& pCanvas,
					::std::unique_ptr<IComponent>& pComposition
				);
				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				void Resize(const ::std::valarray<double>& vertex) override;
				void Rescale(const ::std::valarray<double>& vertex) override;

				void Render();

			private:
				const ::std::unique_ptr<Graphics::Palette> m_pPalette;
				bool m_first;
			};
		}
	}
}