#include "Vban.h"

using namespace ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates::Static;

Vban::Vban(
	Graphics::Canvas& canvas
) : IUpdate{ canvas } {
	set_Color(get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive);
}

void Vban::Update(const int& state) {
	set_Color((state
		? get_Canvas()
			.get_Palette()
				.get_Theme()
					.SecondaryActive
		: get_Canvas()
			.get_Palette()
				.get_Theme()
					.Inactive));
}