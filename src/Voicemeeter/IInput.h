#pragma once

#include "IChannel.h"

namespace Voicemeeter {
	class Input : public IChannel {
	public:
		Input(const Input&) = delete;
		Input(Input&&) = delete;

		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;

	private:
		Input() = default;

		~Input() = default;
	};
}