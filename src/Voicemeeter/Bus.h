#pragma once

#include <array>
#include <type_traits>
#include "estd/type_traits.h"
#include <utility>

#include "Strip.h"

namespace Voicemeeter {
	template<typename Specification, typename TLine, typename TStrip>
	class Bus {
		static_assert(
			::std::is_base_of_v<Strip<typename Specification::Strip, TLine>, TStrip>,
			"TStrip must be derived from Strip");
		//static_assert(
		//	::std::is_move_constructible_v<TStrip>,
		//	"TStrip must be move constructible");

	public:
		template<typename... Args,
			::std::enable_if_t<
				::estd::are_same<TStrip, Args...>(),
				bool> = true>
		inline explicit Bus(
			Args&& ...args
		) : m_cStrip{ ::std::forward<Args>(args)... } {

		};
		Bus() = delete;
		Bus(const Bus&) = delete;
		inline Bus(Bus&& other) = default;

		inline ~Bus() = default;

		Bus& operator=(const Bus&) = delete;
		Bus& operator=(Bus&&) = delete;

		inline auto begin() const {
			return m_cStrip.begin();
		};
		inline auto end() const {
			return m_cStrip.end();
		};

	private:
		::std::array<TStrip, Specification::Width> m_cStrip;
	};
}