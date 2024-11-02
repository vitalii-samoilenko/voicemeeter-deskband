#pragma once

#include "IBus.h"
#include "INetwork.h"
#include "IRange.h"

namespace Voicemeeter {
	enum class Type : long {
		Voicemeeter = 1L,
		Banana = 2L,
		Potato = 3L
	};

	template<
		typename TInput, typename TInputIterator,
		typename TOutput, typename TOutputIterator>
	class IMixer {
	public:
		IMixer(const IMixer&) = delete;
		IMixer(IMixer&&) = delete;

		IMixer& operator=(const IMixer&) = delete;
		IMixer& operator=(IMixer&&) = delete;

		virtual Type get_Type() const = 0;
		virtual const INetwork& get_Network() = 0;
		virtual const IRange<TInput, TInputIterator>& get_Inputs() const = 0;
		virtual const IRange<TOutput, TOutputIterator>& get_Outputs() const = 0;
		virtual const IRange<TOutput, TOutputIterator>& get_Plugs(const TInput& input) const = 0;
		virtual const IRange<TInput, TInputIterator>& get_Plugs(const TOutput& input) const = 0;
		virtual void set_Plug(const TInput& input, const TOutput& output, bool value) = 0;

	protected:
		IMixer() = default;

		~IMixer() = default;
	};
}