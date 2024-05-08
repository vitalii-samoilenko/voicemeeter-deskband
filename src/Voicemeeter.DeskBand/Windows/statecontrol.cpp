#include "statecontrol.h"

using namespace Voicemeeter::DeskBand::Windows;

std::once_flag StateControl::CtlStateClassGuard{};

StateControl::StateControl(
	Panel& rPnlParent
) : Control{
		CtlStateClassGuard,
		L"Voicemeeter.DeskBand.StateControl",
		rPnlParent
	}
  , m_state{ 0L } {

	}