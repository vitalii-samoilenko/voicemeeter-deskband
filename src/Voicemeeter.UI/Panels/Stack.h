#pragma once

#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "estd/type_traits.h"

#include "../Direction.h"
#include "../IPanel.h"
#include "../IComponent.h"

namespace Voicemeeter {
	namespace UI {
		namespace Panels {
			template<
				Direction Direction,
				typename TScale>
			class Stack : public IPanel {
			public:
				template<typename TIterator,
					::std::enable_if_t<
						::std::is_same_v<
							::estd::iterator_value_type_t<TIterator>,
							::std::unique_ptr<IComponent>>,
						bool> = true>
				Stack(
					TIterator begin,
					TIterator end,
					const TScale& scale = {}
				) : m_vertex{}
				  , m_baseVertex { begin->get_BaseSize() }
				  , m_cpComponent{}
				  , m_scale{ scale } {
					m_baseVertex[static_cast<size_t>(Direction)] = ::std::numeric_limits<double>::max();

					for (; begin != end; ++begin) {
						const ::std::valarray<double>& componentBaseVertex{ begin->get_BaseSize() };

						auto maxVertex = m_baseVertex < componentBaseVertex;
						m_baseVertex[maxVertex] = componentBaseVertex[maxVertex];

						m_cpComponent.emplace_back(::std::move(*begin));
					}

					Stack::Rescale(m_baseVertex);

					m_baseVertex[static_cast<size_t>(Direction)] = m_vertex[static_cast<size_t>(Direction)];
				};
				Stack() = delete;
				Stack(const Stack&) = delete;
				Stack(Stack&&) = delete;

				~Stack() = default;

				Stack& operator=(const Stack&) = delete;
				Stack& operator=(Stack&&) = delete;

				virtual void set_Focus(bool value) override {

				};
				virtual const ::std::valarray<double>& get_Position() const override {
					return m_cpComponent.front()
						->get_Position();
				};
				virtual const ::std::valarray<double>& get_Size() const override {
					return m_vertex;
				};
				virtual const ::std::valarray<double>& get_BaseSize() const override {
					return m_baseVertex;
				};

				virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						pComponent->Redraw(point, vertex);
					}
				};
				virtual void Rescale(const ::std::valarray<double>& vertex) override {
					::std::valarray<double> availableVertex{ m_baseVertex * m_scale(m_baseVertex, vertex) };
					::std::valarray<double> componentPoint{ Stack::get_Position() };

					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						pComponent->Move(componentPoint);
						pComponent->Rescale(availableVertex);

						const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

						componentPoint[static_cast<size_t>(Direction)] += componentVertex[static_cast<size_t>(Direction)];
						availableVertex[static_cast<size_t>(Direction)] -= componentVertex[static_cast<size_t>(Direction)];
					}

					m_vertex = availableVertex;
					m_vertex[static_cast<size_t>(Direction)] = componentPoint[static_cast<size_t>(Direction)]
						- Stack::get_Position()[static_cast<size_t>(Direction)];
				}
				virtual void Move(const ::std::valarray<double>& point) override {
					::std::valarray<double> componentPoint{ point };

					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						pComponent->Move(componentPoint);

						componentPoint[static_cast<size_t>(Direction)] += pComponent->get_Size()
							[static_cast<size_t>(Direction)];
					}
				}
				virtual bool MouseLDown(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseLDown(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseLDouble(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseMDown(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseMDown(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseMDouble(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseMDouble(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseRDown(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseRDown(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseRDouble(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseRDouble(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseWheel(point, delta)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseMove(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseMove(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseLUp(const ::std::valarray<double>& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseLUp(point)) {
							return true;
						}
					}
					return false;
				};

			private:
				::std::valarray<double> m_vertex;
				::std::valarray<double> m_baseVertex;
				::std::vector<::std::unique_ptr<IComponent>> m_cpComponent;
				const TScale m_scale;
			};
		}
	}
}