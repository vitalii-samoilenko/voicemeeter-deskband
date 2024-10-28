#pragma once

#include "IChannel.h"

namespace Voicemeeter {
	class IInput : public IChannel {
	public:
		IInput(const IInput&) = delete;
		IInput(IInput&&) = delete;

		IInput& operator=(const IInput&) = delete;
		IInput& operator=(IInput&&) = delete;

	private:
		IInput() = default;

		~IInput() = default;
	};
}