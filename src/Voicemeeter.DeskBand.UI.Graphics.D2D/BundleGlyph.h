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
					template<typename Bundle, typename Param = void>
					class BundleGlyph final : public Glyph {
						static_assert(
							::std::conditional_t<
								::std::is_void_v<Param>,
								::estd::is_invocable_r<void, Func, Canvas&, ::linear_algebra::vector, ::linear_algebra::vector>,
								::estd::is_invocable_r<void, Func, Canvas&, ::linear_algebra::vector, ::linear_algebra::vector, Param>>(),
							"Func must be invocable with Canvas&, vector, vector and Param arguments and void return type");

					public:
						template<
							::std::enable_if_t<
								::std::is_void_v<Param>,
								bool> = true>
						BundleGlyph(
							Canvas& canvas,
							::linear_algebra::vector baseVertex,
							Bundle bundle
						) : Glyph{ canvas, baseVertex }
						  , m_bundle{ ::std::move(bundle) } {

						};
						template<
							::std::enable_if_t<
								::std::negation_v<::std::is_void_v<Param>>,
								bool> = true>
						BundleGlyph(
							Canvas& canvas,
							::linear_algebra::vector baseVertex,
							Bundle bundle
						) : Glyph{ canvas, baseVertex }
						  , m_param{}
						  , m_bundle{ ::std::move(bundle) } {

						};
						BundleGlyph() = delete;
						BundleGlyph(const BundleGlyph&) = delete;
						BundleGlyph(BundleGlyph&&) = delete;

						~BundleGlyph() = default;

						BundleGlyph& operator=(const BundleGlyph&) = delete;
						BundleGlyph& operator=(BundleGlyph&&) = delete;

						template<
							::std::enable_if_t<
								::std::negation_v<::std::is_void_v<Param>>,
								bool> = true>
						inline void set_Param(::estd::remove_cvref_t<Param> param) {
							m_param = std::move(param);
						}

					protected:
						template<
							::std::enable_if_t<
								::std::is_void_v<Param>,
								bool> = true>
						virtual void OnDraw(::linear_algebra::vector point, ::linear_algebra::vector vertex) override {
							m_bundle(m_canvas, point, vertex);
						};
						template<
							::std::enable_if_t<
								::std::negation_v<::std::is_void_v<Param>>,
								bool> = true>
						virtual void OnDraw(::linear_algebra::vector point, ::linear_algebra::vector vertex) override {
							m_bundle(m_canvas, point, vertex, m_param);
						};

					private:
						::estd::remove_cvref_t<Param> m_param;
						Bundle m_bundle;
					};
				}
			}
		}
	}
}