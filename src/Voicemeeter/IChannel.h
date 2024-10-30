#pragma once

namespace Voicemeeter {
	class IChannel {
	public:
		IChannel(const IChannel&) = delete;
		IChannel(IChannel&&) = delete;

		IChannel& operator=(const IChannel&) = delete;
		IChannel& operator=(IChannel&&) = delete;

		virtual double get_Level() const = 0;

	protected:
		IChannel() = default;

		~IChannel() = default;
	};
}