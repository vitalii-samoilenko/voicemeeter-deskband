#ifndef VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_TRANSPARENT_HPP
#define VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_TRANSPARENT_HPP

#include <utility>

#include "wheel.hpp"

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Interactivity {
				template<typename TGlyph>
				class Transparent : public TGlyph {
				public:
					template<typename... Args>
					inline explicit Transparent(Args &&...args)
						: TGlyph{ ::std::forward<Args>(args) ... } {

					};
					Transparent(Transparent const &) = delete;
					Transparent(Transparent &&) = delete;

					inline ~Transparent() = default;

					Transparent & operator=(Transparent const &) = delete;
					Transparent & operator=(Transparent &&) = delete;

					inline void Focus(Focus mode) {

					};
					inline bool MouseLDown(vector_t const &point) {
						return false;
					};
					inline bool MouseLDouble(vector_t const &point) {
						return false;
					};
					inline bool MouseLUp(vector_t const &point) {
						return false;
					};
					inline bool MouseMDown(vector_t const &point) {
						return false;
					};
					inline bool MouseMDouble(vector_t const &point) {
						return false;
					};
					inline bool MouseRDown(vector_t const &point) {
						return false;
					};
					inline bool MouseRDouble(vector_t const &point) {
						return false;
					};
					inline bool MouseWheel(vector_t const &point, num_t delta) {
						return false;
					};
					inline bool MouseMove(vector_t const &point) {
						return false;
					};
				};
			}
		}
	}
}

#endif
