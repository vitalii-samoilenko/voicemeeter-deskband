#pragma once

#include <vector>

#include "IInput.h"
#include "IOutput.h"

namespace Voicemeeter {
	class IMixer {
	public:
		IMixer(const IMixer&) = delete;
		IMixer(IMixer&&) = delete;

		IMixer& operator=(const IMixer&) = delete;
		IMixer& operator=(IMixer&&) = delete;

		virtual bool get_Vban() const = 0;
		virtual void set_Vban(bool value) = 0;
		virtual bool get_Plug(const IInput& input, const IOutput& output);
		virtual void set_Plug(const IInput& input, const IOutput& output);

	private:
		IMixer() = default;

		~IMixer() = default;
	};
}