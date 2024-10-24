#include <algorithm>
#include <limits>

#include "StackPanel.h"

using namespace ::Voicemeeter::DeskBand::UI::Panels;

template<>
void StackPanel<Direction::Right>::OnRescale(const ::linear_algebra::vector& vertex) {
	double scale{
		::std::min<double>(
			static_cast<double>(vertex.x) / m_baseVertex.x,
			static_cast<double>(vertex.y) / m_baseVertex.y)
	};

	::linear_algebra::vector actualVertex{
		0,
		static_cast<int>(m_baseVertex.y * scale)
	};
	::linear_algebra::vector componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(pComponent->get_BaseSize() * scale);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
		actualVertex.x += componentVertex.x;
	}

	m_vertex = actualVertex;
}
template<>
void StackPanel<Direction::Down>::OnRescale(const ::linear_algebra::vector& vertex) {
	double scale{
		::std::min<double>(
			static_cast<double>(vertex.x) / m_baseVertex.x,
			static_cast<double>(vertex.y) / m_baseVertex.y)
	};

	::linear_algebra::vector actualVertex{
		static_cast<int>(m_baseVertex.x * scale),
		0
	};
	::linear_algebra::vector componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(pComponent->get_BaseSize() * scale);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
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

template<>
void StackPanel<Direction::Right>::Arrange() {
	::linear_algebra::vector virtualVertex{
		::std::numeric_limits<int>::max(),
		0
	};
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		virtualVertex.y = ::std::max<int>(virtualVertex.y, pComponent->get_BaseSize().y);
	}

	::linear_algebra::vector actualVertex{
		0,
		virtualVertex.y
	};
	::linear_algebra::vector componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.x += componentVertex.x;
		actualVertex.x += componentVertex.x;
	}

	m_baseVertex = actualVertex;
	m_vertex = actualVertex;
}
template<>
void StackPanel<Direction::Down>::Arrange() {
	::linear_algebra::vector virtualVertex{
		0,
		::std::numeric_limits<int>::max()
	};
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		virtualVertex.x = ::std::max<int>(virtualVertex.x, pComponent->get_BaseSize().x);
	}

	::linear_algebra::vector actualVertex{
		virtualVertex.x,
		0
	};
	::linear_algebra::vector componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::linear_algebra::vector& componentVertex{ pComponent->get_Size() };

		componentPoint.y += componentVertex.y;
		actualVertex.y += componentVertex.y;
	}

	m_baseVertex = actualVertex;
	m_vertex = actualVertex;
}