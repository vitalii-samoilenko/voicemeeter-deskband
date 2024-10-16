#pragma once

#include <forward_list>
#include <memory>

#include "estd/linear_algebra.h"

#include "../IPanel.h"
#include "../IComponent.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Panels {
				enum class Direction {
					Right = 0,
					Bottom = 1
				};

				template<Direction>
				class StackPanel : public IPanel {
				public:
					StackPanel() = default;
					StackPanel(const StackPanel&) = delete;
					StackPanel(StackPanel&&) = delete;

					~StackPanel() = default;

					StackPanel& operator=(const StackPanel&) = delete;
					StackPanel& operator=(StackPanel&&) = delete;

					const virtual linear_algebra::vector get_Position() override {
						return m_point;
					};
					const virtual linear_algebra::vector get_Size() override {
						return m_vertex;
					};

					virtual void Redraw(linear_algebra::vector point, linear_algebra::vector vertex) override {
						linear_algebra::vector abs_vertex{ point + vertex };
						if (linear_algebra::is_outside(m_point, abs_vertex)) {
							return;
						}
						abs_vertex = m_point + m_vertex;
						if (linear_algebra::is_outside(point, abs_vertex)) {
							return;
						}

						for (const std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
							pComponent->Redraw(point, vertex);
						}
					};

				private:
					linear_algebra::vector m_point;
					linear_algebra::vector m_vertex;

					std::forward_list<std::unique_ptr<IComponent>> m_cpComponent;
				};
			}
		}
	}
}