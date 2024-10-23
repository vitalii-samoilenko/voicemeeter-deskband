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
					template<typename TBundle, typename TParam = void>
					class BundleGlyph final : public Glyph {
						static_assert(
							::std::conditional_t<
								::std::is_void_v<TParam>,
								::estd::is_invocable_r<void, TFunc, Canvas&, const ::linear_algebra::vector&, const ::linear_algebra::vector&>,
								::estd::is_invocable_r<void, TFunc, Canvas&, const ::linear_algebra::vector&, const ::linear_algebra::vector&, ::estd::remove_cvref_t<TParam>&>>(),
							"TBundle must be invocable with Canvas&, const vector&, const vector& and TParam& arguments and void return type");

					public:
						template<
							::std::enable_if_t<
								::std::is_void_v<TParam>,
								bool> = true>
						BundleGlyph(
							Canvas& canvas,
							const ::linear_algebra::vector& baseVertex,
							TBundle bundle
						) : Glyph{ canvas, baseVertex }
						  , m_bundle{ ::std::move(bundle) } {

						};
						template<
							::std::enable_if_t<
								::std::negation_v<::std::is_void_v<TParam>>,
								bool> = true>
						BundleGlyph(
							Canvas& canvas,
							const ::linear_algebra::vector& baseVertex,
							TBundle bundle
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
								::std::negation_v<::std::is_void_v<TParam>>,
								bool> = true>
						inline void set_Param(::estd::remove_cvref_t<Param>& param) {
							m_param = std::move(param);
						}

					protected:
						template<
							::std::enable_if_t<
								::std::is_void_v<TParam>,
								bool> = true>
						virtual void OnDraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override {
							m_bundle(m_canvas, point, vertex);
						};
						template<
							::std::enable_if_t<
								::std::negation_v<::std::is_void_v<TParam>>,
								bool> = true>
						virtual void OnDraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override {
							m_bundle(m_canvas, point, vertex, m_param);
						};

					private:
						::estd::remove_cvref_t<TParam> m_param;
						TBundle m_bundle;
					};
				}
			}
		}
	}
}