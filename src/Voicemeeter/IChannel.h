#pragma once

#include <string>

namespace Voicemeeter {
	class IChannel {
	public:
		IChannel(const IChannel&) = delete;
		IChannel(IChannel&&) = delete;

		IChannel& operator=(const IChannel&) = delete;
		IChannel& operator=(IChannel&&) = delete;

		virtual ::std::wstring get_Label() const = 0;
		virtual bool get_Mute() const = 0;
		virtual void set_Mute(bool value) = 0;
		virtual double get_Level() const = 0;
		virtual void set_Level() = 0;

	protected:
		IChannel() = default;

		~IChannel() = default;
	};
}