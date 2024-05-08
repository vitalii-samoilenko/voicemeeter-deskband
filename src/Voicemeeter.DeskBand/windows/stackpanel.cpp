#include "stackpanel.h"

#include "wrappers.h"
#include "resultcodes.h"

using namespace Voicemeeter::DeskBand::Windows;

template<orientation O>
std::once_flag StackPanel<O>::PnlStackClassGuard{};

template<>
LRESULT StackPanel<orientation::right>::OnSize(UINT w, UINT h) {
	UINT left{ 0U };
	for (const pack_type& rPack : get_rPacks()) {
		const std::unique_ptr<Control>& rpCtrl{ std::get<pControl>(rPack) };
		if (rpCtrl->get_hWnd()) {
			const ratio_type& rRatio{ std::get<ratio>(rPack) };

			UINT right{ h * std::get<width>(rRatio) / std::get<height>(rRatio) };
			if (!right) {
				right = max(0U, w - left);
			}
			wMoveWindow(rpCtrl->get_hWnd(), left, 0U, right, h, FALSE);
			left += right;
		}
	}

	return LRESULT_CODES::OK;
}

template<>
StackPanel<orientation::right>::StackPanel(
	Window& rWndParent
) : Panel{
		PnlStackClassGuard,
		L"Voicemeeter.DeskBand.StackPanel<right>",
		rWndParent
	} {

	}
