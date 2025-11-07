#ifndef VOICEMEETER_CHERRY_HPP
#define VOICEMEETER_CHERRY_HPP

#include <stdexcept>
#include <tuple>
#include <utility>

#include "Voicemeeter/Adapters/Multiclient/Channel.hpp"
#include "Voicemeeter/Adapters/Multiclient/Mixer.hpp"
#include "Voicemeeter/Adapters/Multiclient/Network.hpp"
#include "Voicemeeter/Adapters/Multiclient/Strip.hpp"
#include "Voicemeeter/Adapters/Network.hpp"
#include "Voicemeeter/Adapters/Strip.hpp"
#include "Voicemeeter/Channel.hpp"
#include "Voicemeeter/Mixer.hpp"

namespace Voicemeeter {
	class CherryLayout {
	public:
		template<typename... Args>
		inline explicit CherryLayout(Args &&...args)
			: _strips{ ::std::forward<Args>(args)... } {

		};
		CherryLayout(CherryLayout const &) = delete;
		CherryLayout(CherryLayout &&) = delete;

		inline ~CherryLayout() = default;

		CherryLayout & operator=(CherryLayout const &) = delete;
		CherryLayout & operator=(CherryLayout &&) = delete;

		enum Strips : size_t {
			P = 0,
			V = 1,
			A1 = 2,
			A2 = 3,
			B1 = 4,
			B2 = 5
		};
		template<size_t I>
		inline auto & get_Strip() {
			return ::std::get<I>(_strips);
		};

	protected:
		static constexpr size_t InputSize{ V - P + 1 };
		static constexpr size_t OutputSize{ B2 - A1 + 1 };

		template<typename TStrip>
		inline size_t get_Index(TStrip const &strip) const {
			if (&::std::get<P>(_strips) == &strip) {
				return P;
			} else if (&::std::get<V>(_strips) == &strip) {
				return V;
			} else if (&::std::get<A1>(_strips) == &strip) {
				return A1;
			} else if (&::std::get<A2>(_strips) == &strip) {
				return A2;
			} else if (&::std::get<B1>(_strips) == &strip) {
				return B1;
			} else if (&::std::get<B2>(_strips) == &strip) {
				return B2;
			}
			throw ::std::out_of_range{ "Unknown strip" };
		};

	private:
		using PIStrip = Adapters::Multiclient::Strip<
			Adapters::Strip<
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>>>;
		using VIStrip = Adapters::Multiclient::Strip<
			Adapters::Strip<
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>>>;
		using POStrip = Adapters::Multiclient::Strip<
			Adapters::Strip<
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>>>;
		using VOStrip = Adapters::Multiclient::Strip<
			Adapters::Strip<
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>,
				Adapters::Multiclient::Channel<
					Channel>>>;

		::std::tuple<
			PIStrip,
			VIStrip,
			POStrip,
			POStrip,
			VOStrip,
			VOStrip
		> _strips;
	};

	using Cherry = Adapters::Multiclient::Network<
		Adapters::Multiclient::Mixer<
			Adapters::Network<
				Mixer<
					CherryLayout
			>>
	>>;
}

#endif
