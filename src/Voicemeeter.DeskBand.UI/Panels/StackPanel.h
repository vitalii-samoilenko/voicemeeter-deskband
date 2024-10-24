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
						::std::enable_if_t<
							::std::is_same_v<
								::estd::iterator_value_type_t<TIterator>,
								::std::unique_ptr<IComponent>>,
							bool> = true>
					StackPanel(
						const ::linear_algebra::vector& baseMarginTopLeft,
						const ::linear_algebra::vector& baseMarginBottomRight,
						TIterator begin,
						TIterator end
					);

					StackPanel() = delete;
					StackPanel(const StackPanel&) = delete;
					StackPanel(StackPanel&&) = delete;

					~StackPanel() = default;

					StackPanel& operator=(const StackPanel&) = delete;
					StackPanel& operator=(StackPanel&&) = delete;

					virtual void MouseMove(const ::linear_algebra::vector& point) override { };
					virtual void MouseLUp(const ::linear_algebra::vector& point) override { };

				protected:
					virtual const ::linear_algebra::vector& OnGet_Position() const override {
						return m_cpComponent.front()->get_Position();
					};
					virtual const ::linear_algebra::vector& OnGet_Size() const override {
						return m_vertex;
					};
					virtual const ::linear_algebra::vector& OnGet_BaseSize() const override {
						return m_baseVertex;
					};

					virtual void OnRedraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							pComponent->Redraw(point, vertex);
						}
					};
					virtual void OnRescale(const ::linear_algebra::vector& vertex) override;
					virtual void OnMove(const ::linear_algebra::vector& point) override;
					virtual void OnMouseLDown(const ::linear_algebra::vector& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseLDown(point)) {
								break;
							}
						}
					};
					virtual void OnMouseRDown(const ::linear_algebra::vector& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseRDown(point)) {
								break;
							}
						}
					};
					virtual void OnMouseWheel(const ::linear_algebra::vector& point, int delta) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseWheel(point, delta)) {
								break;
							}
						}
					};

				private:
					::linear_algebra::vector m_vertex;
					::linear_algebra::vector m_baseVertex;
					::std::vector<::std::unique_ptr<IComponent>> m_cpComponent;
				};

				template<>
				template<typename TIterator,
					::std::enable_if_t<
						::std::is_same_v<
							::estd::iterator_value_type_t<TIterator>,
							::std::unique_ptr<IComponent>>,
						bool>>
				StackPanel<Direction::Right>::StackPanel(
					const ::linear_algebra::vector& baseMarginTopLeft,
					const ::linear_algebra::vector& baseMarginBottomRight,
					TIterator begin,
					TIterator end
				) : Panel{ baseMarginTopLeft, baseMarginBottomRight }
				  , m_baseVertex{}
				  , m_cpComponent{} {
					::linear_algebra::vector virtual_vertex{};
					for (; begin != end; ++begin) {
						virtual_vertex.y = ::std::max<int>(virtual_vertex.y, (*begin)->get_BaseSize().y);
						m_cpComponent.emplace_back(::std::move(*begin));
					}

					OnRescale(virtual_vertex);

					m_baseVertex = m_vertex;
				};
				template<>
				template<typename TIterator,
					::std::enable_if_t<
						::std::is_same_v<
							::estd::iterator_value_type_t<TIterator>,
							::std::unique_ptr<IComponent>>,
						bool>>
				StackPanel<Direction::Down>::StackPanel(
					const ::linear_algebra::vector& baseMarginTopLeft,
					const ::linear_algebra::vector& baseMarginBottomRight,
					TIterator begin,
					TIterator end
				) : Panel{ baseMarginTopLeft, baseMarginBottomRight }
				  , m_baseVertex{}
				  , m_cpComponent{} {
					::linear_algebra::vector virtualVertex{};
					for (; begin != end; ++begin) {
						virtualVertex.x = ::std::max<int>(virtualVertex.x, (*begin)->get_BaseSize().x);
						m_cpComponent.emplace_back(::std::move(*begin));
					}
					m_cpComponent.front()->Move(baseMarginTopLeft);

					OnRescale(virtualVertex);

					m_baseVertex = m_vertex;
				};

				template<>
				void StackPanel<Direction::Right>::OnRescale(const ::linear_algebra::vector& vertex);
				template<>
				void StackPanel<Direction::Down>::OnRescale(const ::linear_algebra::vector& vertex);
				template<>
				void StackPanel<Direction::Right>::OnMove(const ::linear_algebra::vector& point);
				template<>
				void StackPanel<Direction::Down>::OnMove(const ::linear_algebra::vector& point);
			};
		}
	}
}