#ifndef VOICEMEETER_CLIENTS_UI_PALETTE_HPP
#define VOICEMEETER_CLIENTS_UI_PALETTE_HPP

#include <memory>

#include "Voicemeeter/UI/Graphics/Palette.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			using Palette = UI::Graphics::Palette;

			class PaletteBuilder {
			public:
				inline PaletteBuilder() = default;
				PaletteBuilder(PaletteBuilder const &) = delete;
				PaletteBuilder(PaletteBuilder &&) = delete;

				inline ~PaletteBuilder() = default;

				PaletteBuilder & operator=(PaletteBuilder const &) = delete;
				PaletteBuilder & operator=(PaletteBuilder &&) = delete;

			protected:
				inline ::std::unique_ptr<Palette> Build() {
					return ::std::make_unique<
						Palette>();
				};
			}
		}
	}
}

#endif
