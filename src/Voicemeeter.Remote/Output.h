#pragma once

#include "Decorators/Amplifier.h"

namespace Voicemeeter {
	namespace Remote {
		class Output : public Decorators::Amplifier<Bus> {
		public:
			using Decorators::Amplifier<Bus>::Amplifier;

			Output(const Output&) = delete;
			Output(Output&&) = delete;

			~Output() = default;

			Output& operator=(const Output&) = delete;
			Output& operator=(Output&&) = delete;
		};
	}
}