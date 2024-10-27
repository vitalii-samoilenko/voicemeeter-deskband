#pragma once

#include "IChannel.h"

namespace Voicemeeter {
	class IOutput : public IChannel {
	public:
		IOutput(const IOutput&) = delete;
		IOutput(IOutput&&) = delete;

		IOutput& operator=(const IOutput&) = delete;
		IOutput& operator=(IOutput&&) = delete;

	private:
		IOutput() = default;

		~IOutput() = default;
	};
}