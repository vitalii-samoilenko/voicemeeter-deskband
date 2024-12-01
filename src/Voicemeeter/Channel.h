#pragma once

#include <array>
#include <utility>

#include "estd/type_traits.h"

#include "Line.h"

namespace Voicemeeter {
	template<typename Specification, typename TLine>
	class Channel {
		static_assert(
			::std::is_base_of_v<Line, TLine>,
			"TLine must be derived from Line");
		static_assert(
			::std::is_move_constructible_v<TLine>,
			"TLine must be move constructible");

	public:
		template<typename... Args,
			::std::enable_if_t<
				::estd::are_same<TLine, Args...>(),
				bool> = true>
		inline explicit Channel(
			 Args&& ...args
		) : m_cLine{ ::std::forward<Args>(args)... } {

		};
		Channel() = delete;
		Channel(const Channel&) = delete;
		inline Channel(Channel&&) = default;

		inline ~Channel() = default;

		Channel& operator=(const Channel&) = delete;
		Channel& operator=(Channel&&) = delete;

		inline auto begin() {
			return m_cLine.begin();
		};
		inline auto end() {
			return m_cLine.end();
		};

	private:
		::std::array<TLine, Specification::Width> m_cLine;
	};
}