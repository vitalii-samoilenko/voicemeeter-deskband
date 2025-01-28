#pragma once

#include <type_traits>
#include <utility>

#include "Voicemeeter.UI/Graphics/IGlyph.h"

#include "Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Graphics {
				template<
					typename TInstrumentation,
					typename TBundle>
				class Glyph : public UI::Graphics::IGlyph {
					using Bundle = Bundle<TInstrumentation>;

					static_assert(
						::std::is_base_of_v<Bundle, TBundle>,
						"TBundle must be derived from Bundle");

				public:
					template<typename... Args>
					inline explicit Glyph(
						Args&& ...args
					) : m_bundle{ ::std::forward<Args>(args)... } {

					};
					Glyph() = delete;
					Glyph(const Glyph&) = delete;
					Glyph(Glyph&&) = delete;

					~Glyph() = default;

					Glyph& operator=(const Glyph&) = delete;
					Glyph& operator=(Glyph&&) = delete;

					inline TBundle& get_Bundle() {
						return m_bundle;
					};
					virtual const ::std::valarray<double>& get_Position() const override {
						return m_bundle.get_Position();
					};
					virtual const ::std::valarray<double>& get_Size() const override {
						return m_bundle.get_Size();
					};
					virtual const ::std::valarray<double>& get_BaseSize() const override {
						return m_bundle.get_BaseSize();
					};

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
						m_bundle.get_Palette()
							.get_Queue() += m_bundle;
					};
					virtual void Move(const ::std::valarray<double>& point) override {
						m_bundle.set_Position(point);
						m_bundle.get_Palette()
							.get_Queue() += m_bundle;
					};
					virtual void Rescale(const ::std::valarray<double>& vertex) override {
						m_bundle.set_Size(vertex);
						m_bundle.get_Palette()
							.get_Queue() += m_bundle;
					};

				private:
					TBundle m_bundle;
				};
			}
		}
	}
}