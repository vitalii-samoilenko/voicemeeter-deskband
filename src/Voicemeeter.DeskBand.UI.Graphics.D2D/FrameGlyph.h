#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.UI/Graphics/IGlyph.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					class FrameGlyph final : public IGlyph {
					public:
						template<typename TIterator,
							::std::enable_if_t<
								::std::is_same_v<
									::estd::iterator_value_type_t<TIterator>,
									::std::unique_ptr<IGlyph>>,
								bool> = true>
						FrameGlyph(
							TIterator begin,
							TIterator end
						) : m_frame{}
						  , m_cpGlyph{} {
							for (; begin != end; ++begin) {
								m_cpGlyph.emplace_back(::std::move(*begin));
							}
						};
						FrameGlyph() = delete;
						FrameGlyph(const FrameGlyph&) = delete;
						FrameGlyph(FrameGlyph&&) = delete;

						~FrameGlyph() = default;

						FrameGlyph& operator=(const FrameGlyph&) = delete;
						FrameGlyph& operator=(FrameGlyph&&) = delete;

						inline void set_Frame(int frame) {
							m_frame = frame;
						}

						virtual const ::linear_algebra::vector& get_Position() const override;
						virtual const ::linear_algebra::vector& get_Size() const override;
						virtual const ::linear_algebra::vector& get_BaseSize() const override;

						virtual void Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override;
						virtual void Move(const ::linear_algebra::vector& point) override;
						virtual void Rescale(const ::linear_algebra::vector& vertex) override;

					private:
						int m_frame;
						::std::vector<::std::unique_ptr<IGlyph>> m_cpGlyph;
					};
				}
			}
		}
	}
}