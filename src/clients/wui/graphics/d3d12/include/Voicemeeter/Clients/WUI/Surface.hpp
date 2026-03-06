#ifndef VOICEMEETER_CLIENTS_WUI_SURFACE_HPP
#define VOICEMEETER_CLIENTS_WUI_SURFACE_HPP

#include <memory>
#include <stdexcept>

#include <windows.h>

#include "WUI/Graphics/Surface.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace WUI {
			using Surface = ::WUI::Graphics::Surface;

			class SurfaceBuilder {
			public:
				using Surface = Surface;

				inline SurfaceBuilder() = default;
				SurfaceBuilder(SurfaceBuilder const &) = delete;
				SurfaceBuilder(SurfaceBuilder &&) = delete;

				inline ~SurfaceBuilder() = default;

				SurfaceBuilder & operator=(SurfaceBuilder const &) = delete;
				SurfaceBuilder & operator=(SurfaceBuilder &&) = delete;

				inline SurfaceBuilder & set_hWnd(HWND value) {
					_hWnd = value;
					return *this;
				};

				inline ::std::unique_ptr<Surface> Build() {
					if (_hWnd == NULL) {
						throw ::std::runtime_error{ "HWND is not set" };
					}
					return ::std::make_unique<
						Surface>(_hWnd);
				};

			private:
				HWND _hWnd;
			};
		}
	}
}

#endif
