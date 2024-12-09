#pragma once

#include "Voicemeeter.UI/Scene.h"

#include "Graphics/Canvas.h"
#include "Trackers/Dirty.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			class Scene : public UI::Scene<Graphics::Canvas> {
				using Base = UI::Scene<Graphics::Canvas>;

			public:
				Scene(
					::std::unique_ptr<Trackers::Dirty>& pDirtyTracker,
					::std::unique_ptr<UI::Trackers::Input>& pInputTracker,
					::std::unique_ptr<UI::Trackers::Focus>& pFocusTracker,
					::std::unique_ptr<Graphics::Canvas>& pCanvas,
					::std::unique_ptr<IComponent>& pComposition
				);
				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
				void Resize(const ::std::valarray<double>& vertex) override;
				void Rescale(const ::std::valarray<double>& vertex) override;
				void Redraw();

			private:
				const ::std::unique_ptr<Trackers::Dirty> m_pDirtyTracker;
				bool m_first;
			};
		}
	}
}