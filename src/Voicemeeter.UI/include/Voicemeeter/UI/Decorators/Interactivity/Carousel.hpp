#ifndef VOICEMEETER_UI_DECORATORS_INTERACTIVITY_CAROUSEL_HPP
#define VOICEMEETER_UI_DECORATORS_INTERACTIVITY_CAROUSEL_HPP

#include <utility>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<typename TStateControl>
				class Carousel : public TStateControl {
				public:
					template<typename... Args>
					inline explicit Carousel(Args &&...args)
						: TStateControl{ ::std::forward<Args>(args)... } {

					};
					Carousel(Carousel const &) = delete;
					Carousel(Carousel &&) = delete;

					inline ~Carousel() = default;

					Carousel & operator=(Carousel const &) = delete;
					Carousel & operator=(Carousel &&) = delete;

					inline void Focus(Focus mode) {

					};
					inline bool MouseLDown(::std::valarray<int> const &point) {
						TStateControl::SetNext();
						return true;
					};
					inline bool MouseLDouble(::std::valarray<int> const &point) {
						TStateControl::SetNext();
						return true;
					};
					inline bool MouseLUp(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseMDown(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseMDouble(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseRDown(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseRDouble(::std::valarray<int> const &point) {
						return true;
					};
					inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
						return true;
					};
					inline bool MouseMove(::std::valarray<int> const &point) {
						return true;
					};
				};
			}
		}
	}
}

#endif
