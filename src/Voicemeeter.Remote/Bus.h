#pragma once

#include <utility>

#include "Voicemeeter/IBus.h"

#include "Channel.h"

namespace Voicemeeter {
	namespace Remote {
		class Bus : public IBus<Channel, RangeIterator<Channel>> {
		public:
			template<typename... Args>
			Bus(
				Mixer& mixer,
				long type,
				long channel,
				Args&& ...args
			) : Bus{ ::std::forward<Args>(args)... } {
				m_cChannel.emplace(
					mixer, type, channel
				);
			}
			explicit Bus(
				const ::std::string& label
			);
			Bus() = delete;
			Bus(const Bus&) = delete;
			Bus(Bus&&) = delete;

			~Bus() = default;

			Bus& operator=(const Bus&) = delete;
			Bus& operator=(Bus&&) = delete;

			virtual const ::std::string& get_Label() const override;
			virtual const Range<Channel>& get_Channels() const override;

			virtual void Update(bool dirty);

		private:
			::std::string m_label;
			Range<Channel> m_cChannel;
		};
	}
}