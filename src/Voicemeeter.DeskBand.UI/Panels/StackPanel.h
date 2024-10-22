#pragma once

#include <algorithm>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "../IPanel.h"
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
				class StackPanel final : public IPanel {
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

					virtual linear_algebra::vector get_Position() const override {
						return m_point;
					};
					virtual linear_algebra::vector get_Size() const override {
						return m_vertex;
					};
					virtual linear_algebra::vector get_BaseSize() const override {
						return m_baseVertex;
					};

					virtual void Redraw(linear_algebra::vector point, linear_algebra::vector vertex) override {
						if (!linear_algebra::is_overlapping(point, vertex, m_point, m_vertex)) {
							return;
						}

						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							pComponent->Redraw(point, vertex);
						}
					};
					virtual void Rescale(linear_algebra::vector vertex) override;
					virtual void Move(linear_algebra::vector point) override;
					virtual bool MouseLDown(linear_algebra::vector point) override {
						if (!linear_algebra::is_inside(point - m_point, m_vertex)) {
							return false;
						}

						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseLDown(point)) {
								break;
							}
						}

						return true;
					};
					virtual bool MouseRDown(linear_algebra::vector point) override {
						if (!linear_algebra::is_inside(point - m_point, m_vertex)) {
							return false;
						}

						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseRDown(point)) {
								break;
							}
						}

						return true;
					};
					virtual bool MouseWheel(linear_algebra::vector point, int delta) override {
						if (!linear_algebra::is_inside(point - m_point, m_vertex)) {
							return false;
						}

						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseWheel(point, delta)) {
								break;
							}
						}

						return true;
					};

				private:
					linear_algebra::vector m_point;
					linear_algebra::vector m_vertex;
					linear_algebra::vector m_baseVertex;

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
				) : m_point{}
				  , m_vertex{}
				  , m_baseVertex{ std::numeric_limits<int>::max(), 0 }
				  , m_cpComponent{} {
					for (; begin != end; ++begin) {
						m_baseVertex.y = std::max(m_baseVertex.y, (*begin)->get_BaseSize().y);

						m_cpComponent.emplace_back(std::move((*begin)));
					}

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
				) : m_point{}
				  , m_vertex{}
				  , m_baseVertex{ 0, std::numeric_limits<int>::max() }
				  , m_cpComponent{} {
					for (; begin != end; ++begin) {
						m_baseVertex.x = std::max(m_baseVertex.x, (*begin)->get_BaseSize().x);

						m_cpComponent.emplace_back(std::move((*begin)));
					}

					Rescale(m_baseVertex);

					m_baseVertex = m_vertex;
				};

				template<>
				void StackPanel<Direction::Right>::Rescale(linear_algebra::vector vertex);
				template<>
				void StackPanel<Direction::Down>::Rescale(linear_algebra::vector vertex);
				template<>
				void StackPanel<Direction::Right>::Move(linear_algebra::vector point);
				template<>
				void StackPanel<Direction::Down>::Move(linear_algebra::vector point);
			};
		}
	}
}