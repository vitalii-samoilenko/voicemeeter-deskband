#pragma once

#include "Voicemeeter.Clients.UI.Cherry/Builder.h"
#include "Voicemeeter.UI.D3D12/Scene.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace D3D12 {
				class Cherry : public UI::Cherry::Builder<Adapters::Multiclient::Cherry, ::Voicemeeter::UI::D3D12::Graphics::Instrumentation> {
					using Builder = UI::Cherry::Builder<Adapters::Multiclient::Cherry, ::Voicemeeter::UI::D3D12::Graphics::Instrumentation>;

				public:
					inline Cherry(
						HWND hWnd,
						HMODULE hModule,
						::Environment::IInputTracker& inputTracker,
						::Environment::ITimer& compositionTimer,
						Adapters::Multiclient::Cherry& mixer
					) : Builder{ inputTracker, compositionTimer, mixer }
					  , m_hWnd{ hWnd }
					  , m_hModule{ hModule } {

					};
					Cherry() = delete;
					Cherry(const Cherry&) = delete;
					Cherry(Cherry&&) = delete;

					inline ~Cherry() = default;

					Cherry& operator=(const Cherry&) = delete;
					Cherry& operator=(Cherry&&) = delete;

					::std::unique_ptr<::Voicemeeter::UI::D3D12::Scene> Build();

				private:
					HWND m_hWnd;
					HMODULE m_hModule;
				};
			}
		}
	}
}