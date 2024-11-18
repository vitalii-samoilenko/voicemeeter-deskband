#pragma once

#include <type_traits>
#include <utility>

#include "../IComponent.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			template<typename TComponent>
			class Margin : public TComponent {
				static_assert(
					::std::is_base_of_v<IComponent, TComponent>,
					"TComponent must be derived from IComponent");

			public:
				template<typename... Args>
				Margin(
					const ::std::valarray<double>& baseMarginTopLeft,
					const ::std::valarray<double>& baseMarginBottomRight,
					Args&&... args
				) : TComponent{ ::std::forward<Args>(args)... }
				  , m_position{ TComponent::get_Position() }
				  , m_vertex{ TComponent::get_BaseSize() + baseMarginTopLeft + baseMarginBottomRight }
				  , m_baseVertex{ m_vertex }
				  , m_marginTopLeft{ baseMarginTopLeft }
				  , m_marginBottomRight{ baseMarginBottomRight }
				  , m_baseMarginTopLeft{ baseMarginTopLeft }
				  , m_baseMarginBottomRight{ baseMarginBottomRight } {
					TComponent::Move(m_position + baseMarginTopLeft);
				}
				Margin() = delete;
				Margin(const Margin&) = delete;
				Margin(Margin&&) = delete;

				~Margin() = default;

				Margin& operator=(const Margin&) = delete;
				Margin& operator=(Margin&&) = delete;

				virtual const ::std::valarray<double>& get_Position() const override {
					return m_position;
				};
				virtual const ::std::valarray<double>& get_Size() const override {
					return m_vertex;
				};
				virtual const ::std::valarray<double>& get_BaseSize() const override {
					return m_baseVertex;
				};

				virtual void Rescale(const ::std::valarray<double>& vertex) override {
					double scale{ (vertex / m_baseVertex).min() };

					m_marginTopLeft = m_baseMarginTopLeft * scale;
					m_marginBottomRight = m_baseMarginBottomRight * scale;

					TComponent::Move(m_position + m_marginTopLeft);
					TComponent::Rescale(vertex - m_marginTopLeft - m_marginBottomRight);

					m_vertex = TComponent::get_Size() + m_marginTopLeft + m_marginBottomRight;
				};
				virtual void Move(const ::std::valarray<double>& point) override {
					m_position = point;

					TComponent::Move(point + m_marginTopLeft);
				};

			private:
				::std::valarray<double> m_position;
				::std::valarray<double> m_vertex;
				::std::valarray<double> m_baseVertex;
				::std::valarray<double> m_marginTopLeft;
				::std::valarray<double> m_marginBottomRight;
				::std::valarray<double> m_baseMarginTopLeft;
				::std::valarray<double> m_baseMarginBottomRight;
			};
		}
	}
}