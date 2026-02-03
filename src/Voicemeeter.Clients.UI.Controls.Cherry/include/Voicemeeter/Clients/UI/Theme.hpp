#ifndef VOICEMEETER_CLIENTS_UI_THEME_HPP
#define VOICEMEETER_CLIENTS_UI_THEME_HPP

#include <memory>

#include "wheel.hpp"

#include "Voicemeeter/UI/Graphics/Theme.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			using Theme = UI::Graphics::Theme;

			class ThemeBuilder {
			public:
				using Theme = Theme;

				ThemeBuilder(ThemeBuilder const &) = delete;
				ThemeBuilder(ThemeBuilder &&) = delete;

				inline ~ThemeBuilder() = default;

				ThemeBuilder & operator=(ThemeBuilder const &) = delete;
				ThemeBuilder & operator=(ThemeBuilder &&) = delete;

				inline ThemeBuilder & set_Inactive(vector_t const &value) {
					_prototype.Inactive = value;
					return *this;
				};
				inline ThemeBuilder & set_Active(vector_t const &value) {
					_prototype.Active = value;
					return *this;
				};
				inline ThemeBuilder & set_Warning(vector_t const &value) {
					_prototype.Warning = value;
					return *this;
				};
				inline ThemeBuilder & set_Error(vector_t const &value) {
					_prototype.Error = value;
					return *this;
				};
				inline ThemeBuilder & set_Information(vector_t const &value) {
					_prototype.Information = value;
					return *this;
				};
				inline ThemeBuilder & set_Neutral(vector_t const &value) {
					_prototype.Neutral = value;
					return *this;
				};
				inline ThemeBuilder & set_EqLow(vector_t const &value) {
					_prototype.EqLow = value;
					return *this;
				};
				inline ThemeBuilder & set_EqMedium(vector_t const &value) {
					_prototype.EqMedium = value;
					return *this;
				};
				inline ThemeBuilder & set_EqHigh(vector_t const &value) {
					_prototype.EqHigh = value;
					return *this;
				};

			protected:
				inline ThemeBuilder() = default;

				inline ::std::unique_ptr<Theme> Build() {
					return ::std::make_unique<
						Theme>(_prototype);
				};

			private:
				Theme _prototype;
			};
		}
	}
}

#endif
