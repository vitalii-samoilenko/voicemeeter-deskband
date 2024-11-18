#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "estd/type_traits.h"

#include "../IGlyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			namespace Glyphs {
				class Frame final : public IGlyph {
				public:
					template<typename TIterator,
						::std::enable_if_t<
							::std::is_same_v<
								::estd::iterator_value_type_t<TIterator>,
								::std::unique_ptr<IGlyph>>,
							bool> = true>
					Frame(
						TIterator begin,
						TIterator end
					) : m_frame{}
					  , m_cpGlyph{} {
						for (; begin != end; ++begin) {
							m_cpGlyph.emplace_back(::std::move(*begin));
						}
					};
					Frame() = delete;
					Frame(const Frame&) = delete;
					Frame(Frame&&) = delete;

					~Frame() = default;

					Frame& operator=(const Frame&) = delete;
					Frame& operator=(Frame&&) = delete;

					inline void set_Frame(int frame) {
						m_frame = frame;
					}

					virtual const ::std::valarray<double>& get_Position() const override;
					virtual const ::std::valarray<double>& get_Size() const override;
					virtual const ::std::valarray<double>& get_BaseSize() const override;

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
					virtual void Move(const ::std::valarray<double>& point) override;
					virtual void Rescale(const ::std::valarray<double>& vertex) override;

				private:
					int m_frame;
					::std::vector<::std::unique_ptr<IGlyph>> m_cpGlyph;
				};
			}
		}
	}
}