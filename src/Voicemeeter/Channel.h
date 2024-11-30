#pragma once

#include <array>
#include <type_traits>
#include "estd/type_traits.h"
#include <utility>

#include "Multiclient/Manager.h"
#include "Line.h"

namespace Voicemeeter {
	template<typename Specification, typename TLine>
	class Channel {
		static_assert(
			::std::is_base_of_v<Line, TLine>,
			"TLine must be derived from Line");
		//static_assert(
		//	::std::is_move_constructible_v<TLine>,
		//	"TLine must be move constructible");

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

		inline auto begin() const {
			return m_cLine.begin();
		};
		inline auto end() const {
			return m_cLine.end();
		};

	private:
		const ::std::array<TLine, Specification::Width> m_cLine;
	};
}