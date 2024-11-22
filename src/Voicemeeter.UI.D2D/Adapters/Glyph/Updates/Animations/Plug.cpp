#include "Plug.h"

using namespace ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Animations;

namespace {
	enum animation_vector : size_t {
		active = 0
	};
}
const ::std::valarray<long long> Plug::g_baseVertex{ 200LL * 1000LL };

Plug::Plug(
	Trackers::IDirty& dirtyTracker,
	Graphics::Canvas& canvas,
	const ::std::wstring& label
) : Animation{ dirtyTracker, canvas } {
	set_Label(label);
	set_Color(get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive);
}

void Plug::OnUpdate(const int& state) {
	get_Velocity()[active] = state
		? 1LL
		: -1LL;
}
void Plug::OnFrame() {
	FLOAT alpha{ static_cast<FLOAT>(get_AnimationSize()[active]) / g_baseVertex[active] };
	const ::D2D1::ColorF& from{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive };
	const ::D2D1::ColorF& to{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.PrimaryActive };
	set_Color(::D2D1::ColorF(
		from.r * (1.F - alpha) + to.r * alpha,
		from.g * (1.F - alpha) + to.g * alpha,
		from.b * (1.F - alpha) + to.b * alpha
	));
}