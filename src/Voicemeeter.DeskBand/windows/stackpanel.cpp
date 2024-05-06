#include "stackpanel.h"

#include "wrappers.h"
#include "resultcodes.h"

using namespace Voicemeeter::DeskBand::Windows;

template<orientation O>
std::once_flag StackPanel<O>::PnlStackClassGuard{};

template<>
LRESULT StackPanel<orientation::right>::OnSize() {
	RECT rc{};
	wGetClientRect(get_hWnd(), &rc);
	LONG pnlRight{ rc.right };

	for (const pack_type& rPack : get_rPacks()) {
		const std::unique_ptr<Control>& rpControl{ std::get<pControl>(rPack) };
		if (rpControl->get_hWnd()) {
			const ratio_type& rRatio{ std::get<ratio>(rPack) };

			rc.right = rc.bottom * std::get<width>(rRatio) / std::get<height>(rRatio);
			if (!rc.right) {
				rc.right = max(0L, pnlRight - rc.left);
			}
			wMoveWindow(rpControl->get_hWnd(), rc.left, rc.top, rc.right, rc.bottom, FALSE);
			rc.left += rc.right;
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
