#pragma once

#include <algorithm>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "../Panel.h"
#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Panels {
				enum class Direction {
					Right = 0,
					Down = 1
				};

				template<Direction>
				class StackPanel final : public Panel {
				public:
					template<typename TIterator,
						std::enable_if_t<
							std::is_same_v<
								estd::iterator_value_type_t<TIterator>,
								std::unique_ptr<IComponent>>,
							bool>
						= true>
					StackPanel(
						TIterator begin,
						TIterator end
					);

					StackPanel() = delete;
					StackPanel(const StackPanel&) = delete;
					StackPanel(StackPanel&&) = delete;

					~StackPanel() = default;

					StackPanel& operator=(const StackPanel&) = delete;
					StackPanel& operator=(StackPanel&&) = delete;

				protected:
					virtual void OnRedraw(linear_algebra::vector point, linear_algebra::vector vertex) override {
						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							pComponent->Redraw(point, vertex);
						}
					};
					virtual void OnRescale(linear_algebra::vector vertex) override;
					virtual void OnMove(linear_algebra::vector point) override;
					virtual void OnMouseLDown(linear_algebra::vector point) override {
						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseLDown(point)) {
								break;
							}
						}
					};
					virtual void OnMouseRDown(linear_algebra::vector point) override {
						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseRDown(point)) {
								break;
							}
						}
					};
					virtual void OnMouseWheel(linear_algebra::vector point, int delta) override {
						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseWheel(point, delta)) {
								break;
							}
						}
					};

				private:
					std::vector<std::unique_ptr<IComponent>> m_cpComponent;
				};

				template<>
				template<typename TIterator,
					std::enable_if_t<
						std::is_same_v<
							estd::iterator_value_type_t<TIterator>,
							std::unique_ptr<IComponent>>,
						bool>>
				StackPanel<Direction::Right>::StackPanel(
					TIterator begin,
					TIterator end
				) : m_cpComponent{} {
					for (; begin != end; ++begin) {
						m_baseVertex.y = std::max(m_baseVertex.y, (*begin)->get_BaseSize().y);

						m_cpComponent.emplace_back(std::move((*begin)));
					}
					m_baseVertex.x = std::numeric_limits<int>::max();

					Rescale(m_baseVertex);

					m_baseVertex = m_vertex;
				};
				template<>
				template<typename TIterator,
					std::enable_if_t<
						std::is_same_v<
							estd::iterator_value_type_t<TIterator>,
							std::unique_ptr<IComponent>>,
					bool>>
				StackPanel<Direction::Down>::StackPanel(
					TIterator begin,
					TIterator end
				) : m_cpComponent{} {
					for (; begin != end; ++begin) {
						m_baseVertex.x = std::max(m_baseVertex.x, (*begin)->get_BaseSize().x);

						m_cpComponent.emplace_back(std::move((*begin)));
					}
					m_baseVertex.y = std::numeric_limits<int>::max();

					Rescale(m_baseVertex);

					m_baseVertex = m_vertex;
				};

				template<>
				void StackPanel<Direction::Right>::OnRescale(linear_algebra::vector vertex);
				template<>
				void StackPanel<Direction::Down>::OnRescale(linear_algebra::vector vertex);
				template<>
				void StackPanel<Direction::Right>::OnMove(linear_algebra::vector point);
				template<>
				void StackPanel<Direction::Down>::OnMove(linear_algebra::vector point);
			};
		}
	}
}