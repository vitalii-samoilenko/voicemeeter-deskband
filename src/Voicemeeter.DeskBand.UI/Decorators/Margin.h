#pragma once

#include <type_traits>
#include <utility>

#include "estd/linear_algebra.h"

#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
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
						const ::linear_algebra::vectord& baseMarginTopLeft,
						const ::linear_algebra::vectord& baseMarginBottomRight,
						Args... args
					) : TComponent{ ::std::move(args)... }
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

					virtual const ::linear_algebra::vectord& get_Position() const override {
						return m_position;
					};
					virtual const ::linear_algebra::vectord& get_Size() const override {
						return m_vertex;
					};
					virtual const ::linear_algebra::vectord& get_BaseSize() const override {
						return m_baseVertex;
					};

					virtual void Rescale(const ::linear_algebra::vectord& vertex) override {
						double scale{
							::std::min<double>(
								vertex.x / m_baseVertex.x,
								vertex.y / m_baseVertex.y)
						};

						m_marginTopLeft = m_baseMarginTopLeft * scale;
						m_marginBottomRight = m_baseMarginBottomRight * scale;

						TComponent::Move(m_position + m_marginTopLeft);
						TComponent::Rescale(vertex - m_marginTopLeft - m_marginBottomRight);

						m_vertex = TComponent::get_Size() + m_marginTopLeft + m_marginBottomRight;
					};
					virtual void Move(const ::linear_algebra::vectord& point) override {
						m_position = point;

						TComponent::Move(point + m_marginTopLeft);
					};

				private:
					::linear_algebra::vectord m_position;
					::linear_algebra::vectord m_vertex;
					::linear_algebra::vectord m_baseVertex;
					::linear_algebra::vectord m_marginTopLeft;
					::linear_algebra::vectord m_marginBottomRight;
					::linear_algebra::vectord m_baseMarginTopLeft;
					::linear_algebra::vectord m_baseMarginBottomRight;
				};
			}
		}
	}
}