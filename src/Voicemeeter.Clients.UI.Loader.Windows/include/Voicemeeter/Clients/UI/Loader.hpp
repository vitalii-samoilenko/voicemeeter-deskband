#ifndef VOICEMEETER_CLIENTS_UI_LOADER_HPP
#define VOICEMEETER_CLIENTS_UI_LOADER_HPP

#include <memory>

#include "Windows/API.hpp"

#include "Voicemeeter/UI/Graphics/Loader.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			using Loader = UI::Graphics::Loader;

			class LoaderBuilder {
			public:
				using Loader = Loader;

				LoaderBuilder(LoaderBuilder const &) = delete;
				LoaderBuilder(LoaderBuilder &&) = delete;

				inline ~LoaderBuilder() = default;

				LoaderBuilder & operator=(LoaderBuilder const &) = delete;
				LoaderBuilder & operator=(LoaderBuilder &&) = delete;

				inline LoaderBuilder & set_hModule(HMODULE value) {
					_hModule = value;
					return *this;
				};

			protected:
				inline LoaderBuilder() = default;

				inline ::std::unique_ptr<Loader> Build() {
					return ::std::make_unique<
						Loader>(_hModule);
				};

			private:
				HMODULE _hModule;
			};
		}
	}
}

#endif
