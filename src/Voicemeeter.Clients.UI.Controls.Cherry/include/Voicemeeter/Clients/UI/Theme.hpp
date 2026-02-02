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
				inline ThemeBuilder() = default;
				ThemeBuilder(ThemeBuilder const &) = delete;
				ThemeBuilder(ThemeBuilder &&) = delete;

				inline ~ThemeBuilder() = default;

				ThemeBuilder & operator=(ThemeBuilder const &) = delete;
				ThemeBuilder & operator=(ThemeBuilder &&) = delete;

				inline void set_Inactive(vector_t const &value) {
					_prototype.Inactive = value;
				};
				inline void set_Active(vector_t const &value) {
					_prototype.Active = value;
				};
				inline void set_Warning(vector_t const &value) {
					_prototype.Warning = value;
				};
				inline void set_Error(vector_t const &value) {
					_prototype.Error = value;
				};
				inline void set_Ok(vector_t const &value) {
					_prototype.Ok = value;
				};
				inline void set_Information(vector_t const &value) {
					_prototype.Information = value;
				};
				inline void set_Neutral(vector_t const &value) {
					_prototype.Neutral = value;
				};
				inline void set_EqLow(vector_t const &value) {
					_prototype.EqLow = value;
				};
				inline void set_EqNormal(vector_t const &value) {
					_prototype.EqNormal = value;
				};
				inline void set_EqHigh(vector_t const &value) {
					_prototype.EqHigh = value;
				};

			protected:
				inline ::std::unique_ptr<Theme> Build() {
					return ::std::make_unique<Theme>(
						_prototype);
				};

			private:
				Theme _prototype;
			};
		}
	}
}

#endif
