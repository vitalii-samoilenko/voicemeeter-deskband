#include <algorithm>

#include "Knob.h"

using namespace ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Animations;

namespace {
	enum animation_vector : size_t {
		level_low = 0, level_medium = 1, level_high = 2,
		gain_danger = 3,
		hold = 4, toggle = 5,
		label = 6
	};
}
const ::std::valarray<long long> Knob::g_baseVertex{
	200LL * 1000LL, 200LL * 1000LL, 200LL * 1000LL,
	200LL * 1000LL,
	200LL * 1000LL, 200LL * 1000LL,
	200LL * 1000LL
};

Knob::Knob(
	Trackers::IDirty& dirtyTracker,
	Graphics::Canvas& canvas,
	const ::std::wstring& label
) : Animation{ dirtyTracker, canvas }
  , m_label{ label }
  , m_gain{ 9000 } {
	set_Label(m_label);
	const ::D2D1::ColorF& color{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive
	};
	set_FrameColor(color);
	set_LabelColor(color);
	set_Angle(m_gain / 100.F);
}

void Knob::OnUpdate(const States::Knob& state) {
	::std::valarray<long long>& velocity{ get_Velocity() };
	if (state.hold) {
		velocity[hold] = 1;
		velocity[label] = 1;
	} else {
		velocity[hold] = -1;
		velocity[label] = -1;
	}
	velocity[toggle] = state.toggle
		? 1
		: -1;
	velocity[gain_danger] = 9000 < state.gain
		? 1
		: -1;
	int level{
		(state.level.size()
			? state.level.max()
			: 0)
	};
	if (level < 5) {
		velocity[level_low] = -1;
		velocity[level_medium] = -1;
		velocity[level_high] = -1;
	} else if (level < 700) {
		velocity[level_low] = 1;
		velocity[level_medium] = -1;
		velocity[level_high] = -1;
	} else if (level < 10000) {
		velocity[level_low] = 1;
		velocity[level_medium] = 1;
		velocity[level_high] = -1;
	} else {
		velocity[level_low] = 1;
		velocity[level_medium] = 1;
		velocity[level_high] = 1;
	}
	m_gain = state.gain;
	set_Angle(m_gain / 100.F);
}
void Knob::OnFrame() {
	auto blend = [](::D2D1::ColorF& dst, const ::D2D1::ColorF& src, FLOAT alpha)->void {
		dst.r = dst.r * (1.F - alpha) + src.r * alpha;
		dst.g = dst.g * (1.F - alpha) + src.g * alpha;
		dst.b = dst.b * (1.F - alpha) + src.b * alpha;
	};
	const ::std::valarray<long long> aVertex{ get_AnimationSize() };
	::D2D1::ColorF result{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive };
	blend(result,
		get_Canvas()
			.get_Palette()
				.get_Theme()
					.EqualizerLow,
		static_cast<FLOAT>(aVertex[level_low]) / g_baseVertex[level_low]);
	blend(result,
		get_Canvas()
			.get_Palette()
				.get_Theme()
					.EqualizerMedium,
		static_cast<FLOAT>(aVertex[level_medium]) / g_baseVertex[level_medium]);
	blend(result,
		get_Canvas()
			.get_Palette()
				.get_Theme()
					.EqualizerHigh,
		static_cast<FLOAT>(aVertex[level_high]) / g_baseVertex[level_high]);
	::D2D1::ColorF gain{ get_Canvas()
		.get_Palette()
			.get_Theme()
				.PrimaryActive };
	blend(gain,
		get_Canvas()
			.get_Palette()
				.get_Theme()
					.Danger,
		static_cast<FLOAT>(aVertex[gain_danger]) / g_baseVertex[gain_danger]);
	blend(result, gain,
		static_cast<FLOAT>(aVertex[hold]) / g_baseVertex[hold]);
	blend(result,
		get_Canvas()
			.get_Palette()
				.get_Theme()
					.Warning,
		static_cast<FLOAT>(aVertex[toggle]) / g_baseVertex[toggle]);
	set_FrameColor(result);
	long long mid{ g_baseVertex[label] / 2 };
	result.a = ::std::abs(static_cast<FLOAT>(aVertex[label] - mid) / mid);
	set_LabelColor(result);
	set_Label((aVertex[label] < mid
		? m_label
		: ::std::to_wstring(::std::abs(
			static_cast<int>(
				::std::floor((m_gain - 9000) / 375.))))));
}