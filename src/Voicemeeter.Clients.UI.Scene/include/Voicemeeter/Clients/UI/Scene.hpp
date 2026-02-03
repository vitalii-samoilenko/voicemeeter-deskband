#ifndef VOICEMEETER_CLIENTS_UI_SCENE_HPP
#define VOICEMEETER_CLIENTS_UI_SCENE_HPP

#include <memory>
#include <utility>

#include "Voicemeeter/UI/Scene.hpp"
#include "Voicemeeter/UI/Trackers/Focus.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			template<
				typename TCanvas,
				typename TComposition>
			using Scene = UI::Scene<
				UI::Trackers::Focus,
				TCanvas,
				TComposition>;

			template<
				TPaletteBuilder,
				TThemeBuilder,
				TGraphicsBuilder,
				TControlsBuilder>
			class SceneBuilder
				: public TPaletteBuilder
				, public TThemeBiulder
				, public TGraphicsBuilder
				, public TControlsBuilder {
			public:
				using Scene = Scene<
					typename TGraphicsBuilder::Graphics,
					typename TControlsBuilder::Controls>;

				inline SceneBuilder() = default;
				SceneBuilder(SceneBuilder const &) = delete;
				SceneBuilder(SceneBuilder &&) = delete;

				inline ~SceneBuilder() = default;

				SceneBuilder & operator=(SceneBuilder const &) = delete;
				SceneBuilder & operator=(SceneBuilder &&) = delete;

				inline ::std::unique_ptr<Scene> Build() {
					auto canvas = TGraphicsBuilder::Build(
						TPaletteBuilder::Build(),
						TThemeBuilder::Build());
					auto focusTracker = ::std::make_unique<UI::Trackers::Focus>();
					auto composition = TControlsBuilder::Build(
						*canvas, *focusTracker);
					return ::std::make_unique<Scene>(
						::std::move(focusTracker),
						::std::move(canvas),
						::std::move(composition));
				};
			};
		}
	}
}

#endif
