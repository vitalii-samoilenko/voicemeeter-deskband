#include <utility>

#include "Component.h"

using namespace ::Voicemeeter::DeskBand::UI;

Component::Component(
	IInputTracker& inputTracker,
	const ::linear_algebra::vector& baseMarginTopLeft,
	const ::linear_algebra::vector& baseMarginBottomRight
) : m_inputTracker{ inputTracker }
  , m_marginTopLeft{ baseMarginTopLeft }
  , m_marginBottomRight{ baseMarginBottomRight }
  , m_baseMarginTopLeft{ baseMarginTopLeft }
  , m_baseMarginBottomRight{ baseMarginBottomRight } {

}

const ::linear_algebra::vector& Component::get_Position() const {
	return OnGet_Position() - m_marginTopLeft;
}
const ::linear_algebra::vector& Component::get_Size() const {
	return OnGet_Size() + m_marginTopLeft + m_marginBottomRight;
}
const ::linear_algebra::vector& Component::get_BaseSize() const {
	return OnGet_BaseSize() + m_baseMarginTopLeft + m_baseMarginBottomRight;
}

void Component::Redraw(const ::linear_algebra::vector& point, const ::linear_algebra::vector& vertex) {
	if (!::linear_algebra::is_overlapping(
			point, vertex,
			OnGet_Position(), OnGet_Size())) {
		return;
	}

	OnRedraw(point, vertex);
}
void Component::Rescale(const ::linear_algebra::vector& vertex) {
	::linear_algebra::vector point{ get_Position() };
	::linear_algebra::vector baseVertex{ get_BaseSize() };

	double scale{
		::std::min<double>(
			static_cast<double>(vertex.x) / baseVertex.x,
			static_cast<double>(vertex.y) / baseVertex.y)
	};

	m_marginTopLeft.x = m_baseMarginTopLeft.x * scale;
	m_marginTopLeft.y = m_baseMarginTopLeft.y * scale;
	m_marginBottomRight.x = m_baseMarginBottomRight.x * scale;
	m_marginBottomRight.y = m_baseMarginBottomRight.y * scale;

	Move(point);
	OnRescale(vertex - m_marginTopLeft - m_marginBottomRight);
}
void Component::Move(const ::linear_algebra::vector& point) {
	OnMove(point + m_marginTopLeft);
}
bool Component::MouseLDown(const ::linear_algebra::vector& point) {
	if (m_inputTracker.IsTracking(*this)) {
		OnMouseLDown(point);

		return true;
	}

	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseLDown(point);
	}

	return true;
}
bool Component::MouseLDouble(const ::linear_algebra::vector& point) {
	if (m_inputTracker.IsTracking(*this)) {
		OnMouseLDouble(point);

		return true;
	}

	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseLDouble(point);
	}

	return true;
}
bool Component::MouseRDown(const ::linear_algebra::vector& point) {
	if (m_inputTracker.IsTracking(*this)) {
		OnMouseRDown(point);

		return true;
	}

	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseRDown(point);
	}

	return true;
}
bool Component::MouseWheel(const ::linear_algebra::vector& point, int delta) {
	if (m_inputTracker.IsTracking(*this)) {
		OnMouseWheel(point, delta);

		return true;
	}

	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseWheel(point, delta);
	}

	return true;
}
bool Component::MouseMove(const ::linear_algebra::vector& point) {
	if (m_inputTracker.IsTracking(*this)) {
		OnMouseMove(point);

		return true;
	}

	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseMove(point);
	}

	return true;
}
bool Component::MouseLUp(const ::linear_algebra::vector& point) {
	if (m_inputTracker.IsTracking(*this)) {
		OnMouseLUp(point);

		return true;
	}

	if (!::linear_algebra::is_inside(
			point - get_Position(),
			get_Size())) {
		return false;
	}

	if (::linear_algebra::is_inside(
			point - OnGet_Position(),
			OnGet_Size())) {
		OnMouseLUp(point);
	}

	return true;
}

bool Component::IsTrackingInput() const {
	return m_inputTracker.IsTracking(*this);
}
void Component::EnableInputTrack() {
	m_inputTracker.EnableInputTrack(*this);
}
void Component::DisableInputTrack() {
	m_inputTracker.DisableInputTrack(*this);
}