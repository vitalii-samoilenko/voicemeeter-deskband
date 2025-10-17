#ifndef VOICEMEETER_GROUP_HPP
#define VOICEMEETER_GROUP_HPP

#include <array>
#include <utility>

namespace Voicemeeter {
	template<size_t Size, typename TStrip>
	class Group {
	public:
		template<typename... Args>
		inline explicit Group(Args &&...args)
			: _strips{ ::std::forward<Args>(args)... } {

		};
		Group() = delete;
		Group(Group const &) = delete;
		Group(Group &&) = delete;

		inline ~Group() = default;

		Group & operator=(Group const &) = delete;
		Group & operator=(Group &&) = delete;

		inline auto begin() {
			return _strips.begin();
		};
		inline auto end() {
			return _strips.end();
		};

	private:
		::std::array<TStrip, Size> _strips;
	};
}

#endif
