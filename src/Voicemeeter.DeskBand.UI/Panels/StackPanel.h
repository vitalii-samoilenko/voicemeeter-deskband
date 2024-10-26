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
						::std::enable_if_t<
							::std::is_same_v<
								::estd::iterator_value_type_t<TIterator>,
								::std::unique_ptr<IComponent>>,
							bool> = true>
					StackPanel(
						TIterator begin,
						TIterator end
					) : m_baseVertex{}
					  , m_cpComponent{}{
						for (; begin != end; ++begin) {
							m_cpComponent.emplace_back(::std::move(*begin));
						}
						Arrange();
					};

					StackPanel() = delete;
					StackPanel(const StackPanel&) = delete;
					StackPanel(StackPanel&&) = delete;

					~StackPanel() = default;

					StackPanel& operator=(const StackPanel&) = delete;
					StackPanel& operator=(StackPanel&&) = delete;

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
								break;
							}
						}
					};
					virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseWheel(point, delta)) {
								break;
							}
						}
					};
					virtual bool MouseMove(const ::linear_algebra::vectord& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseMove(point)) {
								break;
							}
						}
					};
					virtual bool MouseLUp(const ::linear_algebra::vectord& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseLUp(point)) {
								break;
							}
						}
					};

				private:
					::linear_algebra::vectord m_vertex;
					::linear_algebra::vectord m_baseVertex;
					::std::vector<::std::unique_ptr<IComponent>> m_cpComponent;

					void Arrange();
				};

				template<>
				void StackPanel<Direction::Right>::Rescale(const ::linear_algebra::vectord& vertex);
				template<>
				void StackPanel<Direction::Down>::Rescale(const ::linear_algebra::vectord& vertex);
				template<>
				void StackPanel<Direction::Right>::Move(const ::linear_algebra::vectord& point);
				template<>
				void StackPanel<Direction::Down>::Move(const ::linear_algebra::vectord& point);

				template<>
				void StackPanel<Direction::Right>::Arrange();
				template<>
				void StackPanel<Direction::Down>::Arrange();
			};
		}
	}
}