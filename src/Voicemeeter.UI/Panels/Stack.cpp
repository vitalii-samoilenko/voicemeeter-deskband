#include <algorithm>
#include <limits>

#include "Stack.h"

using namespace ::Voicemeeter::UI;
using namespace ::Voicemeeter::UI::Panels;

template<>
void Stack<Direction::Right>::Rescale(const ::std::valarray<double>& vertex) {
	double scale{ (vertex / m_baseVertex).min() };

	::std::valarray<double> actualVertex{
		0.,
		m_baseVertex[1] * scale
	};
	::std::valarray<double> componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		const ::std::valarray<double>& componentBaseVertex{ pComponent->get_BaseSize() };

		pComponent->Move(componentPoint);
		pComponent->Rescale(componentBaseVertex * (scale * m_baseVertex[1] / componentBaseVertex[1]));

		const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

		componentPoint[0] += componentVertex[0];
		actualVertex[0] += componentVertex[0];
	}

	m_vertex = actualVertex;
}
template<>
void Stack<Direction::Down>::Rescale(const ::std::valarray<double>& vertex) {
	double scale{ (vertex / m_baseVertex).min() };

	::std::valarray<double> actualVertex{
		m_baseVertex[0] * scale,
		0.
	};
	::std::valarray<double> componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		const ::std::valarray<double>& componentBaseVertex{ pComponent->get_BaseSize() };

		pComponent->Move(componentPoint);
		pComponent->Rescale(componentBaseVertex * (scale * m_baseVertex[0] / componentBaseVertex[0]));

		const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

		componentPoint[1] += componentVertex[1];
		actualVertex[1] += componentVertex[1];
	}

	m_vertex = actualVertex;
}
template<>
void Stack<Direction::Right>::Move(const ::std::valarray<double>& point) {
	::std::valarray<double> componentPoint{ point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);

		const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

		componentPoint[0] += componentVertex[0];
	}
}
template<>
void Stack<Direction::Down>::Move(const ::std::valarray<double>& point) {
	::std::valarray<double> componentPoint{ point };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);

		const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

		componentPoint[1] += componentVertex[1];
	}
}

template<>
void Stack<Direction::Right>::Arrange() {
	::std::valarray<double> virtualVertex{
		::std::numeric_limits<double>::max(),
		0.
	};
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		virtualVertex[1] = ::std::max<double>(virtualVertex[1], pComponent->get_BaseSize()[1]);
	}

	::std::valarray<double> actualVertex{
		0.,
		virtualVertex[1]
	};
	::std::valarray<double> componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

		componentPoint[0] += componentVertex[0];
		actualVertex[0] += componentVertex[0];
	}

	m_baseVertex = actualVertex;
	m_vertex = actualVertex;
}
template<>
void Stack<Direction::Down>::Arrange() {
	::std::valarray<double> virtualVertex{
		0.,
		::std::numeric_limits<double>::max()
	};
	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		virtualVertex[0] = ::std::max<double>(virtualVertex[0], pComponent->get_BaseSize()[0]);
	}

	::std::valarray<double> actualVertex{
		virtualVertex[0],
		0.
	};
	::std::valarray<double> componentPoint{ m_cpComponent.front()->get_Position() };

	for (const ::std::unique_ptr<IComponent>& pComponent : m_cpComponent) {
		pComponent->Move(componentPoint);
		pComponent->Rescale(virtualVertex);

		const ::std::valarray<double>& componentVertex{ pComponent->get_Size() };

		componentPoint[1] += componentVertex[1];
		actualVertex[1] += componentVertex[1];
	}

	m_baseVertex = actualVertex;
	m_vertex = actualVertex;
}