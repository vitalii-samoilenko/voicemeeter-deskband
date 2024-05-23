#include "atlas.h"

#include <tuple>

#include "../../atlas.h"
#include "../wrappers.h"

using namespace Voicemeeter::DeskBand::Windows::Presentation;

static constexpr size_t OUTPUT_D{ 46 };
using id_mp_rsrc = std::tuple<size_t, size_t, int>;
enum id_mp_rsrc_members : size_t {
	image_id = 0,
	mipmap = 1,
	resource_id = 2
};
static constexpr LPCWSTR LP_PIXEL_NAME{ L"PIXEL" };
static constexpr id_mp_rsrc P_MAP[]{
	id_mp_rsrc{	Atlas::out_a_act,	0, IDR_OUT_A_ACT_0		},
	id_mp_rsrc{	Atlas::out_a_act,	1, IDR_OUT_A_ACT_1		},
	id_mp_rsrc{	Atlas::out_b_act,	0, IDR_OUT_B_ACT_0		},
	id_mp_rsrc{	Atlas::out_b_act,	1, IDR_OUT_B_ACT_1		},
	id_mp_rsrc{	Atlas::out_a_inact,	0, IDR_OUT_A_INACT_0	},
	id_mp_rsrc{	Atlas::out_a_inact,	1, IDR_OUT_A_INACT_1	},
	id_mp_rsrc{	Atlas::out_b_inact,	0, IDR_OUT_B_INACT_0	},
	id_mp_rsrc{	Atlas::out_b_inact,	1, IDR_OUT_B_INACT_1	},
};

Atlas::Atlas()
	: m_ppImage{} {
	for (const id_mp_rsrc& map : P_MAP) {
		Image& img{ m_ppImage[std::get<mipmap>(map)][std::get<image_id>(map)] };
		img.Width = img.Height = static_cast<size_t>(pow(2., MIPMAP_COUNT - 1 - std::get<mipmap>(map))) * OUTPUT_D;

		HRSRC hRsrc{ wFindResourceW(NULL, MAKEINTRESOURCEW(std::get<resource_id>(map)), LP_PIXEL_NAME) };
		HGLOBAL hGlobal{ wLoadResource(NULL, hRsrc) };
		img.pData = wLockResource(hGlobal);
	}
}