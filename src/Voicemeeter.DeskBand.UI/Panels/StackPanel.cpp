#include <limits>

#include "StackPanel.h"

using namespace ::Voicemeeter::DeskBand::UI::Panels;

template<>
::linear_algebra::vector StackPanel<Direction::Right>::OnRescale(::linear_algebra::vector vertex) {
	vertex.x = ::std::numeric_limits<int>::max();
	::linear_algebra::vector point{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);
		pComponent->Rescale(vertex);

		point.x += pComponent->get_Size().x;
	}

	vertex.x = point.x - m_cpComponent.front()->get_Position().x;

	return vertex;
}
template<>
::linear_algebra::vector StackPanel<Direction::Down>::OnRescale(::linear_algebra::vector vertex) {
	vertex.y = ::std::numeric_limits<int>::max();
	::linear_algebra::vector point{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);
		pComponent->Rescale(vertex);

		point.y += pComponent->get_Size().y;
	}

	vertex.y = point.y - m_cpComponent.front()->get_Position().y;

	return vertex;
}
template<>
void StackPanel<Direction::Right>::OnMove(::linear_algebra::vector point) {
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);

		point.x += pComponent->get_Size().x;
	}
}
template<>
void StackPanel<Direction::Down>::OnMove(::linear_algebra::vector point) {
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(point);

		point.y += pComponent->get_Size().y;
	}
}