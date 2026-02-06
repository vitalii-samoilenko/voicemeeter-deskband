#ifndef VOICEMEETER_UI_DECORATORS_INTERACTIVITY_CAROUSEL_HPP
#define VOICEMEETER_UI_DECORATORS_INTERACTIVITY_CAROUSEL_HPP

#include <utility>

#include "wheel.hpp"

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<typename TCircularControl>
				class Carousel : public TCircularControl {
				public:
					template<typename ...Args>
					inline explicit Carousel(Args &&...args)
						: TCircularControl{ ::std::forward<Args>(args) ... } {

					};
					Carousel(Carousel const &) = delete;
					Carousel(Carousel &&) = delete;

					inline ~Carousel() = default;

					Carousel & operator=(Carousel const &) = delete;
					Carousel & operator=(Carousel &&) = delete;

					inline void set_Focus(Focus value) {

					};

					inline bool MouseLDown(vector_t const &point) {
						TCircularControl::set_NextState();
						return true;
					};
					inline bool MouseLDouble(vector_t const &point) {
						TCircularControl::set_NextState();
						return true;
					};
					inline bool MouseLUp(vector_t const &point) {
						return true;
					};
					inline bool MouseMDown(vector_t const &point) {
						return true;
					};
					inline bool MouseMDouble(vector_t const &point) {
						return true;
					};
					inline bool MouseRDown(vector_t const &point) {
						return true;
					};
					inline bool MouseRDouble(vector_t const &point) {
						return true;
					};
					inline bool MouseWheel(vector_t const &point, num_t delta) {
						return true;
					};
					inline bool MouseMove(vector_t const &point) {
						return true;
					};
				};
			}
		}
	}
}

#endif
