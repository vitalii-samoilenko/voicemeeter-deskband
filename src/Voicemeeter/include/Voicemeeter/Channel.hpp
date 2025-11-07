#ifndef VOICEMEETER_CHANNEL_HPP
#define VOICEMEETER_CHANNEL_HPP

namespace Voicemeeter {
	class Channel {
	public:
		inline Channel() = default;
		Channel(Channel const &) = delete;
		Channel(Channel &&) = delete;

		inline ~Channel() = default;

		Channel & operator=(Channel const &) = delete;
		Channel & operator=(Channel &&) = delete;

		inline int get_Level() const {
			return _level;
		};
		inline void set_Level(int value) {
			_level = value;
		};

	private:
		int _level;
	};
}

#endif
