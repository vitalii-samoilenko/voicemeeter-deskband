#include <limits>

#include "StackPanel.h"

using namespace ::Voicemeeter::DeskBand::UI::Panels;

template<>
void StackPanel<Direction::Right>::OnRescale(linear_algebra::vector vertex) {
	vertex.x = ::std::numeric_limits<int>::max();
	linear_algebra::vector point{ m_point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);
		pComponent->Rescale(vertex);

		point.x += pComponent->get_Size().x;
	}

	m_vertex.x = point.x - m_point.x;
	m_vertex.y = vertex.y;
}
template<>
void StackPanel<Direction::Down>::OnRescale(linear_algebra::vector vertex) {
	vertex.y = ::std::numeric_limits<int>::max();
	linear_algebra::vector point{ m_point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);
		pComponent->Rescale(vertex);

		point.y += pComponent->get_Size().y;
	}

	m_vertex.x = vertex.x;
	m_vertex.y = point.y - m_point.y;
}
template<>
void StackPanel<Direction::Right>::OnMove(linear_algebra::vector point) {
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);

		point.x += pComponent->get_Size().x;
	}
}
template<>
void StackPanel<Direction::Down>::OnMove(linear_algebra::vector point) {
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);

		point.y += pComponent->get_Size().y;
	}
}