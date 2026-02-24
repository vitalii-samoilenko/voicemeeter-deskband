#ifndef VOICEMEETER_CLIENTS_UI_THEME_HPP
#define VOICEMEETER_CLIENTS_UI_THEME_HPP

#include <memory>

#include "wheel.hpp"

#include "Voicemeeter/UI/Graphics/Theme.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			using Theme = ::Voicemeeter::UI::Graphics::Theme;

			class ThemeBuilder {
			public:
				using Theme = Theme;

				ThemeBuilder(ThemeBuilder const &) = delete;
				ThemeBuilder(ThemeBuilder &&) = delete;

				inline ~ThemeBuilder() = default;

				ThemeBuilder & operator=(ThemeBuilder const &) = delete;
				ThemeBuilder & operator=(ThemeBuilder &&) = delete;

				inline ThemeBuilder & set_Inactive(vector_t const &value) {
					_srgba.Inactive = value;
					return *this;
				};
				inline ThemeBuilder & set_Active(vector_t const &value) {
					_srgba.Active = value;
					return *this;
				};
				inline ThemeBuilder & set_Warning(vector_t const &value) {
					_srgba.Warning = value;
					return *this;
				};
				inline ThemeBuilder & set_Error(vector_t const &value) {
					_srgba.Error = value;
					return *this;
				};
				inline ThemeBuilder & set_Information(vector_t const &value) {
					_srgba.Information = value;
					return *this;
				};
				inline ThemeBuilder & set_Neutral(vector_t const &value) {
					_srgba.Neutral = value;
					return *this;
				};
				inline ThemeBuilder & set_EqLow(vector_t const &value) {
					_srgba.EqLow = value;
					return *this;
				};
				inline ThemeBuilder & set_EqMedium(vector_t const &value) {
					_srgba.EqMedium = value;
					return *this;
				};
				inline ThemeBuilder & set_EqHigh(vector_t const &value) {
					_srgba.EqHigh = value;
					return *this;
				};

			protected:
				inline ThemeBuilder() = default;

				template<typename TPalette>
				inline ::std::unique_ptr<Theme> Build(
					TPalette &palette) {
					auto theme = ::std::make_unique<Theme>();
					theme->Inactive = palette
						.Premultiply(palette
						.Linearize(_srgba.Inactive));
					theme->Active = palette
						.Premultiply(palette
						.Linearize(_srgba.Active));
					theme->Warning = palette
						.Premultiply(palette
						.Linearize(_srgba.Warning));
					theme->Error = palette
						.Premultiply(palette
						.Linearize(_srgba.Error));
					theme->Information = palette
						.Premultiply(palette
						.Linearize(_srgba.Information));
					theme->Neutral = palette
						.Premultiply(palette
						.Linearize(_srgba.Neutral));
					theme->EqLow = palette
						.Premultiply(palette
						.Linearize(_srgba.EqLow));
					theme->EqMedium = palette
						.Premultiply(palette
						.Linearize(_srgba.EqMedium));
					theme->EqHigh = palette
						.Premultiply(palette
						.Linearize(_srgba.EqHigh));
					return theme;
			};

			private:
				Theme _srgba;
			};
		}
	}
}

#endif
