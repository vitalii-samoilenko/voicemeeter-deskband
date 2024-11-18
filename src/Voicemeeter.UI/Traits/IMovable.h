#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Traits {
			class IMovable {
			public:
				IMovable(const IMovable&) = delete;
				IMovable(IMovable&&) = delete;

				IMovable& operator=(const IMovable&) = delete;
				IMovable& operator=(IMovable&&) = delete;

				virtual void Move(const ::std::valarray<double>& point) = 0;

			protected:
				IMovable() = default;

				~IMovable() = default;
			};
		}
	}
}