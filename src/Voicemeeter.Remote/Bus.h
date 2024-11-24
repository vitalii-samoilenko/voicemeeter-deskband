#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Voicemeeter/IBus.h"

#include "Channel.h"

namespace Voicemeeter {
	namespace Remote {
		class Bus : public IBus<::std::vector<::std::unique_ptr<Channel>>> {
		public:
			template<typename... Args>
			Bus(
				const Mixer& mixer,
				LevelType type,
				long channel,
				Args&& ...args
			) : Bus{ ::std::forward<Args>(args)... } {
				m_cpChannel.push_back(::std::move(
					::std::make_unique<Channel>(
						mixer, type, channel
					)
				));
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
			virtual const ::std::vector<::std::unique_ptr<Channel>>& get_Channels() const override;

			virtual void Update(bool dirty);

		private:
			const ::std::string m_label;
			::std::vector<::std::unique_ptr<Channel>> m_cpChannel;
		};
	}
}