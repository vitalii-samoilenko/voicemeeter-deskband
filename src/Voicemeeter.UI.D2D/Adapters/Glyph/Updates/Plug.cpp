#include "Plug.h"

using namespace ::Voicemeeter::UI::D2D::Adapters::Glyph::Updates;

Plug::Plug(
	Graphics::Canvas& canvas,
	const ::std::wstring& label
) : IUpdate{ canvas } {
	set_Label(label);
	set_Color(get_Canvas()
		.get_Palette()
			.get_Theme()
				.Inactive);
}

void Plug::Update(const int& state) {
	set_Color((state
		? get_Canvas()
			.get_Palette()
				.get_Theme()
					.PrimaryActive
		: get_Canvas()
			.get_Palette()
				.get_Theme()
					.Inactive));
}