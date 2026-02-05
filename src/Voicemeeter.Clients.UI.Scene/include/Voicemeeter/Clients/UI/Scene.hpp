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
				typename TFocusTracker,
				typename TCanvas,
				typename TComposition>
			using Scene = UI::Scene<
				TFocusTracker,
				TCanvas,
				TComposition>;

			template<
				typename TFocusTrackerBuilder,
				typename TLoaderBuilder,
				typename TPaletteBuilder,
				typename TThemeBuilder,
				typename TCanvasBuilder,
				typename TCompositionBuilder>
			class SceneBuilder
				: public TFocusTrackerBuilder
				, public TLoaderBuilder
				, public TPaletteBuilder
				, public TThemeBiulder
				, public TCanvasBuilder
				, public TCompositionBuilder {
			public:
				using Scene = Scene<
					typename TFocusTrackerBuilder::FocusTracker,
					typename TCanvasBuilder::Canvas,
					typename TCompositionBuilder::Composition>;

				inline SceneBuilder() = default;
				SceneBuilder(SceneBuilder const &) = delete;
				SceneBuilder(SceneBuilder &&) = delete;

				inline ~SceneBuilder() = default;

				SceneBuilder & operator=(SceneBuilder const &) = delete;
				SceneBuilder & operator=(SceneBuilder &&) = delete;

				inline TFocusTrackerBuilder & get_FocusTrackerBuilder() {
					return *this;
				};
				inline TLoaderBuilder & get_LoaderBuilder() {
					return *this;
				};
				inline TPaletteBuilder & get_PaletteBuilder() {
					return *this;
				};
				inline TThemeBuilder & get_ThemeBuilder() {
					return *this;
				};
				inline TCanvasBuilder & get_CanvasBuilder() {
					return *this;
				};
				inline TCompositionBuilder & get_CompositionBuilder() {
					return *this;
				};

				inline ::std::unique_ptr<Scene> Build() {
					auto focusTracker = TFocusTrackerBuilder::Build();
					auto canvas = TCanvasBuilder::Build(
						TLoaderBuilder::Build(),
						TPaletteBuilder::Build(),
						TThemeBuilder::Build());
					auto composition = TControlsBuilder::Build(
						*canvas, *focusTracker);
					return ::std::make_unique<
						Scene>(
						::std::move(focusTracker),
						::std::move(canvas),
						::std::move(composition));
				};
			};
		}
	}
}

#endif
