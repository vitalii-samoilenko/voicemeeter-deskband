#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Traits {
			class IScalable {
			public:
				IScalable(const IScalable&) = delete;
				IScalable(IScalable&&) = delete;

				IScalable& operator=(const IScalable&) = delete;
				IScalable& operator=(IScalable&&) = delete;

				virtual const ::std::valarray<double>& get_BaseSize() const = 0;

				virtual void Rescale(const ::std::valarray<double>& vertex) = 0;

			protected:
				IScalable() = default;

				~IScalable() = default;
			};
		}
	}
}