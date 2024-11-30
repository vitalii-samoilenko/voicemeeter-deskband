#pragma once

#include "Adapters/Amplifier.h"
#include "Adapters/Mute.h"

namespace Voicemeeter {
	template<
		typename Specification,
		typename TLine>
	using Strip = Adapters::Mute<Specification, TLine,
		Adapters::Amplifier<Specification, TLine,
			Channel<Specification, TLine>>>;
}