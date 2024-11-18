#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Traits {
			class IInteractive {
			public:
				IInteractive(const IInteractive&) = delete;
				IInteractive(IInteractive&&) = delete;

				IInteractive& operator=(const IInteractive&) = delete;
				IInteractive& operator=(IInteractive&&) = delete;

				virtual void set_Focus(bool value) = 0;

				virtual bool MouseLDown(const ::std::valarray<double>& point) = 0;
				virtual bool MouseLDouble(const ::std::valarray<double>& point) = 0;
				virtual bool MouseMDown(const ::std::valarray<double>& point) = 0;
				virtual bool MouseMDouble(const ::std::valarray<double>& point) = 0;
				virtual bool MouseRDown(const ::std::valarray<double>& point) = 0;
				virtual bool MouseRDouble(const ::std::valarray<double>& point) = 0;
				virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) = 0;
				virtual bool MouseMove(const ::std::valarray<double>& point) = 0;
				virtual bool MouseLUp(const ::std::valarray<double>& point) = 0;

			protected:
				IInteractive() = default;

				~IInteractive() = default;
			};
		}
	}
}