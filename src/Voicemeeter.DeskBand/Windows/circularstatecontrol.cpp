#include "circularstatecontrol.h"

#include "wrappers.h"
#include "resultcodes.h"

using namespace Voicemeeter::DeskBand::Windows;

LRESULT CircularStateControl::OnLButtonDown(LONG x, LONG y) {
	if (get_rScene()
			.ContainsPoint(x, y)
	) {
		set_state((get_state() + 1) % m_mod);

		wInvalidateRect(get_hWnd(), NULL, FALSE);
	}

	return LRESULT_CODES::OK;
}

CircularStateControl::CircularStateControl(
	Panel& rPnlParent,
	LONG mod
) : StateControl{
		rPnlParent
	}
  , m_mod{ mod } {

	}