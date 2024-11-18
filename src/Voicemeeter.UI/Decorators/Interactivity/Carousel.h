#pragma once

#include <type_traits>
#include <utility>

#include "../../Controls/Carousel.h"
#include "../../IFocusTracker.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<typename TCarousel>
				class Carousel : public TCarousel {
					static_assert(
						::std::is_base_of_v<Controls::Carousel, TCarousel>,
						"TCarousel must be derived from Carousel");

				public:
					template<typename... Args>
					explicit Carousel(
						IFocusTracker& focusTracker,
						Args&& ...args
					) : TCarousel{ ::std::forward<Args>(args)... }
					  , m_focusTracker{ focusTracker } {

					}
					Carousel() = delete;
					Carousel(const Carousel&) = delete;
					Carousel(Carousel&&) = delete;

					~Carousel() = default;

					Carousel& operator=(const Carousel&) = delete;
					Carousel& operator=(Carousel&&) = delete;

					virtual bool MouseLDown(const ::std::valarray<double>& point) override {
						m_focusTracker.set_Track(component, true);

						TCarousel::SetNext();

						return true;
					}
					virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
						TCarousel::SetNext();

						return true;
					}

				private:
					IFocusTracker& m_focusTracker;
				};
			}
		}
	}
}