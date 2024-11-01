#pragma once

#include "Decorators/Amplifier.h"

namespace Voicemeeter {
	namespace Remote {
		class Input : public Decorators::Amplifier<Bus> {
		public:
			using Decorators::Amplifier<Bus>::Amplifier;

			Input(const Input&) = delete;
			Input(Input&&) = delete;

			~Input() = default;

			Input& operator=(const Input&) = delete;
			Input& operator=(Input&&) = delete;
		};
	}
}