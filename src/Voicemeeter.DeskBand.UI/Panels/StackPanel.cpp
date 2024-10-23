#include <algorithm>
#include <limits>

#include "StackPanel.h"

using namespace ::Voicemeeter::DeskBand::UI::Panels;

template<>
void StackPanel<Direction::Right>::OnRescale(const ::linear_algebra::vector& vertex) {
	::linear_algebra::vector virtualVertex{
		::std::numeric_limits<int>::max(),
		vertex.y
	};
	::linear_algebra::vector actualVertex{};
	::linear_algebra::vector componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
		actualVertex.x += componentVertex.x;
		actualVertex.y = ::std::max(actualVertex.y, componentVertex.y);
	}

	m_vertex = actualVertex;
}
template<>
void StackPanel<Direction::Down>::OnRescale(const ::linear_algebra::vector& vertex) {
	::linear_algebra::vector virtualVertex{
		vertex.x,
		::std::numeric_limits<int>::max()
	};
	::linear_algebra::vector actualVertex{};
	::linear_algebra::vector componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
		actualVertex.x = ::std::max(actualVertex.x, componentVertex.x);
		actualVertex.y += componentVertex.y;
	}

	m_vertex = actualVertex;
}
template<>
void StackPanel<Direction::Right>::OnMove(const ::linear_algebra::vector& point) {
	::linear_algebra::vector componentPoint{ point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
	}
}
template<>
void StackPanel<Direction::Down>::OnMove(const ::linear_algebra::vector& point) {
	::linear_algebra::vector componentPoint{ point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
	}
}