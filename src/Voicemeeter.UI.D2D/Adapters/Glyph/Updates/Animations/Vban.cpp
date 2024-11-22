#include "Vban.h"

using namespace ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Animations;

namespace {
	enum animation_vector : size_t {
		active = 0
	};
}
const ::std::valarray<long long> Vban::g_baseVertex{ 200LL * 1000LL };

Vban::Vban(
	Trackers::IDirty& dirtyTracker,
	Graphics::Canvas& canvas
) : Animation{ dirtyTracker, canvas } {
	set_Color(get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive);
}

void Vban::OnUpdate(const int& state) {
	get_Velocity()[active] = state
		? 1LL
		: -1LL;
}
void Vban::OnFrame() {
	FLOAT alpha{ static_cast<FLOAT>(get_AnimationSize()[active]) / g_baseVertex[active] };
	const ::D2D1::ColorF& from{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive };
	const ::D2D1::ColorF& to{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.SecondaryActive };
	set_Color(::D2D1::ColorF(
		from.r * (1.F - alpha) + to.r * alpha,
		from.g * (1.F - alpha) + to.g * alpha,
		from.b * (1.F - alpha) + to.b * alpha
	));
}