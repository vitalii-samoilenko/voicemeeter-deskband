#pragma once

#include <type_traits>
#include <utility>

#include "estd/linear_algebra.h"
#include "estd/type_traits.h"

#include "Glyph.h"
#include "Canvas.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					template<typename TBundle>
					class BundleGlyph final : public Glyph {
						static_assert(
							::estd::is_invocable_r<void, TBundle, const Canvas&, const ::linear_algebra::vectord&, const ::linear_algebra::vectord&>(),
							"TBundle must be invocable with const Canvas&, const vector& and const vector& arguments and void return type");

					public:
						BundleGlyph(
							Canvas& canvas,
							const ::linear_algebra::vectord& baseVertex,
							TBundle bundle
						) : Glyph{ canvas, baseVertex }
						  , m_bundle{ ::std::move(bundle) } {

						};
						BundleGlyph() = delete;
						BundleGlyph(const BundleGlyph&) = delete;
						BundleGlyph(BundleGlyph&&) = delete;

						~BundleGlyph() = default;

						BundleGlyph& operator=(const BundleGlyph&) = delete;
						BundleGlyph& operator=(BundleGlyph&&) = delete;

						const TBundle& get_Bundle() {
							return m_bundle;
						};

					protected:
						virtual void OnDraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
							m_bundle(get_Canvas(), point, vertex);
						};

					private:
						TBundle m_bundle;
					};
				}
			}
		}
	}
}