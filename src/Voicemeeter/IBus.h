#pragma once

#include <string>

namespace Voicemeeter {
	template<typename TChannelRange>
	class IBus {
	public:
		IBus(const IBus&) = delete;
		IBus(IBus&&) = delete;

		IBus& operator=(const IBus&) = delete;
		IBus& operator=(IBus&&) = delete;

		virtual const ::std::string& get_Label() const = 0;
		virtual const TChannelRange& get_Channels() const = 0;

	protected:
		IBus() = default;

		~IBus() = default;
	};
}