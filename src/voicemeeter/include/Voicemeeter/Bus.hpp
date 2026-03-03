#ifndef VOICEMEETER_BUS_HPP
#define VOICEMEETER_BUS_HPP

#include <tuple>
#include <utility>

namespace Voicemeeter {
	template<typename ...TChannels>
	class Bus {
	public:
		template<typename ...Args>
		inline explicit Bus(Args &&...args)
			: _channels{ ::std::forward<Args>(args) ... } {

		};
		Bus(Bus const &) = delete;
		Bus(Bus &&) = delete;

		inline ~Bus() = default;

		Bus & operator=(Bus const &) = delete;
		Bus & operator=(Bus &&) = delete;

		template<size_t I>
		inline auto & get_Channel() {
			return ::std::get<I>(_channels);
		};

	private:
		::std::tuple<TChannels ...> _channels;
	};
}

#endif
