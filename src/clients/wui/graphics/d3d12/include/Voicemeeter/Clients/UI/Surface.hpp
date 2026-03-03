#ifndef VOICEMEETER_CLIENTS_UI_SURFACE_HPP
#define VOICEMEETER_CLIENTS_UI_SURFACE_HPP

#include <memory>
#include <stdexcept>

#include "Windows/API.hpp"

#include "Voicemeeter/UI/Graphics/Surface.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			using Surface = ::Voicemeeter::UI::Graphics::Surface;

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
