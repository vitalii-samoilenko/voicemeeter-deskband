#include <algorithm>
#include <limits>

#include "Stack.h"

using namespace ::Voicemeeter::DeskBand::UI::Panels;

template<>
void Stack<Direction::Right>::Rescale(const ::linear_algebra::vectord& vertex) {
	double scale{
		::std::min<double>(
			vertex.x / m_baseVertex.x,
			vertex.y / m_baseVertex.y)
	};

	::linear_algebra::vectord actualVertex{
		0,
		m_baseVertex.y * scale
	};
	::linear_algebra::vectord componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(pComponent->get_BaseSize() * scale);

		const ::linear_algebra::vectord& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
		actualVertex.x += componentVertex.x;
	}

	m_vertex = actualVertex;
}
template<>
void Stack<Direction::Down>::Rescale(const ::linear_algebra::vectord& vertex) {
	double scale{
		::std::min<double>(
			vertex.x / m_baseVertex.x,
			vertex.y / m_baseVertex.y)
	};

	::linear_algebra::vectord actualVertex{
		m_baseVertex.x * scale,
		0
	};
	::linear_algebra::vectord componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(pComponent->get_BaseSize() * scale);

		const ::linear_algebra::vectord& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
		actualVertex.y += componentVertex.y;
	}

	m_vertex = actualVertex;
}
template<>
void Stack<Direction::Right>::Move(const ::linear_algebra::vectord& point) {
	::linear_algebra::vectord componentPoint{ point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);

		const ::linear_algebra::vectord& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
	}
}
template<>
void Stack<Direction::Down>::Move(const ::linear_algebra::vectord& point) {
	::linear_algebra::vectord componentPoint{ point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);

		const ::linear_algebra::vectord& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
	}
}

template<>
void Stack<Direction::Right>::Arrange() {
	::linear_algebra::vectord virtualVertex{
		::std::numeric_limits<double>::max(),
		0
	};
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		virtualVertex.y = ::std::max<double>(virtualVertex.y, pComponent->get_BaseSize().y);
	}

	::linear_algebra::vectord actualVertex{
		0,
		virtualVertex.y
	};
	::linear_algebra::vectord componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::linear_algebra::vectord& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
		actualVertex.x += componentVertex.x;
	}

	m_baseVertex = actualVertex;
	m_vertex = actualVertex;
}
template<>
void Stack<Direction::Down>::Arrange() {
	::linear_algebra::vectord virtualVertex{
		0,
		::std::numeric_limits<double>::max()
	};
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		virtualVertex.x = ::std::max<double>(virtualVertex.x, pComponent->get_BaseSize().x);
	}

	::linear_algebra::vectord actualVertex{
		virtualVertex.x,
		0
	};
	::linear_algebra::vectord componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::linear_algebra::vectord& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
		actualVertex.y += componentVertex.y;
	}

	m_baseVertex = actualVertex;
	m_vertex = actualVertex;
}