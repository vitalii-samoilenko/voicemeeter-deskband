#pragma once

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
				virtual void Rescale(const ::std::valarray<double>& vertex) override;
				virtual void Move(const ::std::valarray<double>& point) override;
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

				void Arrange();
			};

			template<>
			void Stack<Direction::Right>::Rescale(const ::std::valarray<double>& vertex);
			template<>
			void Stack<Direction::Down>::Rescale(const ::std::valarray<double>& vertex);
			template<>
			void Stack<Direction::Right>::Move(const ::std::valarray<double>& point);
			template<>
			void Stack<Direction::Down>::Move(const ::std::valarray<double>& point);

			template<>
			void Stack<Direction::Right>::Arrange();
			template<>
			void Stack<Direction::Down>::Arrange();
		};
	}
}