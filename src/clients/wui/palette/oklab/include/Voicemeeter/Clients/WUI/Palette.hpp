#ifndef VOICEMEETER_CLIENTS_WUI_PALETTE_HPP
#define VOICEMEETER_CLIENTS_WUI_PALETTE_HPP

#include <memory>

#include "WUI/Graphics/Palette.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace WUI {
			using Palette = ::WUI::Graphics::Palette;

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
			};
		}
	}
}

#endif
