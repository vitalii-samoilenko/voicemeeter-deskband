#pragma once

#include "IBus.h"
#include "INetwork.h"
#include "IRange.h"

namespace Voicemeeter {
	template<
		typename TInput, typename TInputIterator,
		typename TOutput, typename TOutputIterator>
	class IMixer : public IRange<TInput, TInputIterator>, public IRange<TOutput, TOutputIterator> {
	public:
		IMixer(const IMixer&) = delete;
		IMixer(IMixer&&) = delete;

		IMixer& operator=(const IMixer&) = delete;
		IMixer& operator=(IMixer&&) = delete;

		virtual const INetwork& get_Network() const = 0;
		virtual const IRange<TOutput, TOutputIterator> get_Plugs(const TInput& input) const = 0;
		virtual const IRange<TInput, TInputIterator> get_Plugs(const TOutput& input) const = 0;
		virtual void set_Plug(const TInput& input, const TOutput& output, bool value) = 0;

	private:
		IMixer() = default;

		~IMixer() = default;
	};
}