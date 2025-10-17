#ifndef VOICEMEETER_GROUP_HPP
#define VOICEMEETER_GROUP_HPP

#include <array>
#include <utility>

namespace Voicemeeter {
	template<size_t Size, typename TBus>
	class Group {
	public:
		template<typename... Args>
		inline explicit Group(Args &&...args)
			: _buses{ ::std::forward<Args>(args)... } {

		};
		Group(Group const &) = delete;
		Group(Group &&) = delete;

		inline ~Group() = default;

		Group & operator=(Group const &) = delete;
		Group & operator=(Group &&) = delete;

		inline auto begin() {
			return _buses.begin();
		};
		inline auto end() {
			return _buses.end();
		};

	private:
		::std::array<TBus, Size> _buses;
	};
}

#endif
