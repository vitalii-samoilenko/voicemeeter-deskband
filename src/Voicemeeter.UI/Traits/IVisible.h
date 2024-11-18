#pragma once

#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Traits {
			class IVisible {
			public:
				IVisible(const IVisible&) = delete;
				IVisible(IVisible&&) = delete;

				IVisible& operator=(const IVisible&) = delete;
				IVisible& operator=(IVisible&&) = delete;

				virtual const ::std::valarray<double>& get_Position() const = 0;
				virtual const ::std::valarray<double>& get_Size() const = 0;

				virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) = 0;

			protected:
				IVisible() = default;

				~IVisible() = default;
			};
		}
	}
}