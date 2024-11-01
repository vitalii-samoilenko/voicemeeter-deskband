#pragma once

#include "estd/type_traits.h"

namespace Voicemeeter {
	template<typename T, typename TIterator>
	class IRange {
		static_assert(
			::std::is_same_v<
				T,
				::estd::iterator_value_type_t<TIterator>>,
			"TIterator must be valid iterator of T");

	public:
		IRange(const IRange&) = delete;
		IRange(IRange&&) = delete;

		IRange& operator=(const IRange&) = delete;
		IRange& operator=(IRange&&) = delete;

		virtual TIterator begin() const = 0;
		virtual TIterator end() const = 0;

	protected:
		IRange() = default;

		~IRange() = default;
	};
}