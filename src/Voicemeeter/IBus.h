#pragma once

#include <string>
#include <type_traits>

#include "IChannel.h"
#include "IRange.h"

namespace Voicemeeter {
	template<typename TChannel, typename TChannelIterator>
	class IBus {
		static_assert(
			::std::is_base_of_v<IChannel, TChannel>,
			"TChannel must be derived from IChannel");

	public:
		IBus(const IBus&) = delete;
		IBus(IBus&&) = delete;

		IBus& operator=(const IBus&) = delete;
		IBus& operator=(IBus&&) = delete;

		virtual const ::std::string& get_Label() const = 0;
		virtual const IRange<TChannel, TChannelIterator>& get_Channels() const = 0;

	protected:
		IBus() = default;

		~IBus() = default;
	};
}