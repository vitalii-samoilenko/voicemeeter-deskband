#include <algorithm>
#include <cmath>

#include "Knob.h"

using namespace ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Static;

Knob::Knob(
	const Graphics::Canvas& canvas,
	const ::std::wstring& label
) : IUpdate{ canvas }
  , m_label{ label } {

}

void Knob::Update(const States::Knob& state) {
	set_Label((state.hold
		? ::std::to_wstring(::std::abs(
			static_cast<int>(
				::std::floor((state.gain - 9000) / 375.))))
		: m_label));
	int level{ state.level.max() };
	set_Color((state.toggle
		? get_Canvas()
			.get_Palette()
				.get_Theme()
					.Warning
		: state.hold
			? 9000 < state.gain
				? get_Canvas()
					.get_Palette()
						.get_Theme()
							.Danger
				: get_Canvas()
					.get_Palette()
						.get_Theme()
							.PrimaryActive
			: level < 5
				? get_Canvas()
					.get_Palette()
						.get_Theme()
							.Inactive
				: level < 700
					? get_Canvas()
						.get_Palette()
							.get_Theme()
								.EqualizerLow
					: level < 10000
						? get_Canvas()
							.get_Palette()
								.get_Theme()
									.EqualizerMedium
						: get_Canvas()
							.get_Palette()
								.get_Theme()
									.EqualizerHigh));
	set_Angle(state.gain / 100.F);
}