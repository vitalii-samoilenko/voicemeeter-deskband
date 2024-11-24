#pragma once

#include <type_traits>
#include <utility>

#include "../IComponent.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			template<
				typename TComponent,
				typename TScale>
			class Margin : public TComponent {
				static_assert(
					::std::is_base_of_v<IComponent, TComponent>,
					"TComponent must be derived from IComponent");

			public:
				template<typename... Args>
				Margin(
					const ::std::valarray<double>& baseMarginPoint,
					const ::std::valarray<double>& baseMarginVertex,
					const TScale& scale = {},
					Args&&... args
				) : TComponent{ ::std::forward<Args>(args)... }
				  , m_position{ TComponent::get_Position() }
				  , m_vertex{ TComponent::get_BaseSize() + baseMarginPoint + baseMarginVertex }
				  , m_baseVertex{ m_vertex }
				  , m_marginPoint{ baseMarginPoint }
				  , m_marginVertex{ baseMarginVertex }
				  , m_baseMarginPoint{ baseMarginPoint }
				  , m_baseMarginVertex{ baseMarginVertex }
				  , m_scale{ scale } {
					TComponent::Move(m_position + m_baseMarginPoint);
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
					const ::std::valarray<double> scale{ m_scale(m_baseVertex, vertex) };

					m_marginPoint = m_baseMarginPoint * scale;
					m_marginVertex = m_baseMarginVertex * scale;

					TComponent::Move(m_position + m_marginPoint);
					TComponent::Rescale(vertex - m_marginPoint - m_marginVertex);

					m_vertex = TComponent::get_Size() + m_marginPoint + m_marginVertex;
				};
				virtual void Move(const ::std::valarray<double>& point) override {
					m_position = point;

					TComponent::Move(point + m_marginPoint);
				};

			private:
				::std::valarray<double> m_position;
				::std::valarray<double> m_vertex;
				const ::std::valarray<double> m_baseVertex;
				::std::valarray<double> m_marginPoint;
				::std::valarray<double> m_marginVertex;
				const ::std::valarray<double> m_baseMarginPoint;
				const ::std::valarray<double> m_baseMarginVertex;
				const TScale m_scale;
			};
		}
	}
}