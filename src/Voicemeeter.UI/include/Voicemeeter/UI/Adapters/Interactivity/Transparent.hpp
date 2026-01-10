#ifndef VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_TRANSPARENT_HPP
#define VOICEMEETER_UI_ADAPTERS_INTERACTIVITY_TRANSPARENT_HPP

#include <utility>
#include <valarray>

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
						: TGlyph{ ::std::forward<Args>(args)... } {

					};
					Transparent(Transparent const &) = delete;
					Transparent(Transparent &&) = delete;

					inline ~Transparent() = default;

					Transparent & operator=(Transparent const &) = delete;
					Transparent & operator=(Transparent &&) = delete;

					inline void Focus(Focus mode) {

					};
					inline bool MouseLDown(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseLDouble(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseLUp(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseMDown(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseMDouble(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseRDown(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseRDouble(::std::valarray<int> const &point) {
						return false;
					};
					inline bool MouseWheel(::std::valarray<int> const &point, int delta) {
						return false;
					};
					inline bool MouseMove(::std::valarray<int> const &point) {
						return false;
					};
				};
			}
		}
	}
}

#endif
