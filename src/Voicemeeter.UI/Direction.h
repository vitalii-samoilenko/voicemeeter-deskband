#pragma once

namespace Voicemeeter {
	namespace UI {
		enum class Direction {
			Right = 0,
			Down = 1
		};

		template<UI::Direction>
		struct Direction_Tag {};
	}
}