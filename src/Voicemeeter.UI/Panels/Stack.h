#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "../IPanel.h"
#include "../IComponent.h"

namespace Voicemeeter {
	namespace UI {
		namespace Panels {
			enum class Direction {
				Right = 0,
				Down = 1
			};

			template<Direction>
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
					TIterator end
				) : m_baseVertex{}
				  , m_cpComponent{}{
					for (; begin != end; ++begin) {
						m_cpComponent.emplace_back(::std::move(*begin));
					}
					Arrange();
				};
				Stack() = delete;
				Stack(const Stack&) = delete;
				Stack(Stack&&) = delete;

				~Stack() = default;

				Stack& operator=(const Stack&) = delete;
				Stack& operator=(Stack&&) = delete;

				virtual const ::linear_algebra::vectord& get_Position() const override {
					return m_cpComponent.front()->get_Position();
				};
				virtual const ::linear_algebra::vectord& get_Size() const override {
					return m_vertex;
				};
				virtual const ::linear_algebra::vectord& get_BaseSize() const override {
					return m_baseVertex;
				};

				virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						pComponent->Redraw(point, vertex);
					}
				};
				virtual void Rescale(const ::linear_algebra::vectord& vertex) override;
				virtual void Move(const ::linear_algebra::vectord& point) override;
				virtual bool MouseLDown(const ::linear_algebra::vectord& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseLDown(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseLDouble(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseRDown(const ::linear_algebra::vectord& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseRDown(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseWheel(point, delta)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseMove(const ::linear_algebra::vectord& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseMove(point)) {
							return true;
						}
					}
					return false;
				};
				virtual bool MouseLUp(const ::linear_algebra::vectord& point) override {
					for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
						if (pComponent->MouseLUp(point)) {
							return true;
						}
					}
					return false;
				};

			private:
				::linear_algebra::vectord m_vertex;
				::linear_algebra::vectord m_baseVertex;
				::std::vector<::std::unique_ptr<IComponent>> m_cpComponent;

				void Arrange();
			};

			template<>
			void Stack<Direction::Right>::Rescale(const ::linear_algebra::vectord& vertex);
			template<>
			void Stack<Direction::Down>::Rescale(const ::linear_algebra::vectord& vertex);
			template<>
			void Stack<Direction::Right>::Move(const ::linear_algebra::vectord& point);
			template<>
			void Stack<Direction::Down>::Move(const ::linear_algebra::vectord& point);

			template<>
			void Stack<Direction::Right>::Arrange();
			template<>
			void Stack<Direction::Down>::Arrange();
		};
	}
}