#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "estd/type_traits.h"
#include "estd/linear_algebra.h"

#include "../Panel.h"
#include "../IComponent.h"
#include "../IInputTracker.h"

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
						IInputTracker& inputTracker,
						const ::linear_algebra::vectord& baseMarginTopLeft,
						const ::linear_algebra::vectord& baseMarginBottomRight,
						TIterator begin,
						TIterator end
					) : Panel{ inputTracker, baseMarginTopLeft, baseMarginBottomRight }
					  , m_baseVertex{}
					  , m_cpComponent{}{
						for (; begin != end; ++begin) {
							m_cpComponent.emplace_back(::std::move(*begin));
						}

						m_cpComponent.front()->Move(baseMarginTopLeft);
						Arrange();
					};

					StackPanel() = delete;
					StackPanel(const StackPanel&) = delete;
					StackPanel(StackPanel&&) = delete;

					~StackPanel() = default;

					StackPanel& operator=(const StackPanel&) = delete;
					StackPanel& operator=(StackPanel&&) = delete;

				protected:
					virtual const ::linear_algebra::vectord& OnGet_Position() const override {
						return m_cpComponent.front()->get_Position();
					};
					virtual const ::linear_algebra::vectord& OnGet_Size() const override {
						return m_vertex;
					};
					virtual const ::linear_algebra::vectord& OnGet_BaseSize() const override {
						return m_baseVertex;
					};

					virtual void OnRedraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							pComponent->Redraw(point, vertex);
						}
					};
					virtual void OnRescale(const ::linear_algebra::vectord& vertex) override;
					virtual void OnMove(const ::linear_algebra::vectord& point) override;
					virtual void OnMouseLDown(const ::linear_algebra::vectord& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseLDown(point)) {
								break;
							}
						}
					};
					virtual void OnMouseLDouble(const ::linear_algebra::vectord& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseLDouble(point)) {
								break;
							}
						}
					};
					virtual void OnMouseRDown(const ::linear_algebra::vectord& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseRDown(point)) {
								break;
							}
						}
					};
					virtual void OnMouseWheel(const ::linear_algebra::vectord& point, int delta) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseWheel(point, delta)) {
								break;
							}
						}
					};
					virtual void OnMouseMove(const ::linear_algebra::vectord& point) override {
						for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							if (pComponent->MouseMove(point)) {
								break;
							}
						}
					};
					virtual void OnMouseLUp(const ::linear_algebra::vectord& point) override {
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
				void StackPanel<Direction::Right>::OnRescale(const ::linear_algebra::vectord& vertex);
				template<>
				void StackPanel<Direction::Down>::OnRescale(const ::linear_algebra::vectord& vertex);
				template<>
				void StackPanel<Direction::Right>::OnMove(const ::linear_algebra::vectord& point);
				template<>
				void StackPanel<Direction::Down>::OnMove(const ::linear_algebra::vectord& point);

				template<>
				void StackPanel<Direction::Right>::Arrange();
				template<>
				void StackPanel<Direction::Down>::Arrange();
			};
		}
	}
}