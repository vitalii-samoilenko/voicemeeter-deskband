#ifndef VOICEMEETER_BUS_HPP
#define VOICEMEETER_BUS_HPP

#include <array>
#include <utility>

namespace Voicemeeter {
	template<size_t Width, typename TChannel>
	class Bus {
	public:
		template<typename... Args>
		inline explicit Bus(Args &&...args)
			: _lines{ ::std::forward<Args>(args)... } {

		};
		Bus() = delete;
		Bus(Bus const &) = delete;
		Bus(Bus &&) = delete;

		inline ~Bus() = default;

		Bus & operator=(Bus const &) = delete;
		Bus & operator=(Bus &&) = delete;

		inline auto begin() {
			return _lines.begin();
		};
		inline auto end() {
			return _lines.end();
		};

	private:
		::std::array<TChannel, Width> _lines;
	};
}

#endif
