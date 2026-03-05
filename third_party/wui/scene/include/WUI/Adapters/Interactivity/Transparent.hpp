#ifndef WUI_ADAPTERS_INTERACTIVITY_TRANSPARENT_HPP
#define WUI_ADAPTERS_INTERACTIVITY_TRANSPARENT_HPP

#include <utility>

#include "math.hpp"

#include "WUI/Focus.hpp"

namespace WUI {
	namespace Adapters {
		namespace Interactivity {
			template<typename TGlyph>
			class Transparent : public TGlyph {
			public:
				template<typename ...Args>
				inline explicit Transparent(Args &&...args)
					: TGlyph{ ::std::forward<Args>(args) ... } {

				};
				Transparent(Transparent const &) = delete;
				Transparent(Transparent &&) = delete;

				inline ~Transparent() = default;

				Transparent & operator=(Transparent const &) = delete;
				Transparent & operator=(Transparent &&) = delete;

				inline void set_Focus(Focus value) {

				};

				inline bool MouseLDown(vec_t const &point) {
					return false;
				};
				inline bool MouseLDouble(vec_t const &point) {
					return false;
				};
				inline bool MouseLUp(vec_t const &point) {
					return false;
				};
				inline bool MouseMDown(vec_t const &point) {
					return false;
				};
				inline bool MouseMDouble(vec_t const &point) {
					return false;
				};
				inline bool MouseRDown(vec_t const &point) {
					return false;
				};
				inline bool MouseRDouble(vec_t const &point) {
					return false;
				};
				inline bool MouseWheel(vec_t const &point, num_t delta) {
					return false;
				};
				inline bool MouseMove(vec_t const &point) {
					return false;
				};
			};
		}
	}
}

#endif
