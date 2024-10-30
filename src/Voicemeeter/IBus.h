#pragma once

#include <string>

#include "IChannel.h"
#include "IRange.h"

namespace Voicemeeter {
	template<typename TChannelIterator>
	class IBus : public IRange<IChannel, TChannelIterator> {
	public:
		IBus(const IBus&) = delete;
		IBus(IBus&&) = delete;

		IBus& operator=(const IBus&) = delete;
		IBus& operator=(IBus&&) = delete;

		virtual const ::std::wstring get_Label() const = 0;

	protected:
		IBus() = default;

		~IBus() = default;
	};
}