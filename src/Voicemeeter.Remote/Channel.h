#pragma once

#include <functional>
#include <vector>

#include "Voicemeeter/IChannel.h"

namespace Voicemeeter {
	namespace Remote {
		class Mixer;

		class Channel : public IChannel {
		public:
			Channel(
				Mixer& mixer,
				long type,
				long channel
			);
			Channel() = delete;
			Channel(const Channel&) = delete;
			Channel(Channel&&) = delete;

			~Channel() = default;

			Channel& operator=(const Channel&) = delete;
			Channel& operator=(Channel&&) = delete;

			virtual double get_Level() const override;
			void on_Level(const ::std::function<void(double)>& callback);

			void Update();

		private:
			Mixer& m_mixer;
			long m_type;
			long m_channel;
			double m_level;
			::std::vector<::std::function<void(double)>> m_cCallback;
		};
	}
}

#include "Range.h"

template<>
struct ::std::iterator_traits<::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Channel>> {
	typedef ::Voicemeeter::Remote::Channel value_type;
};