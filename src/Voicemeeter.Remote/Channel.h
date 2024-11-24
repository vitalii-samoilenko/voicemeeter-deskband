#pragma once

#include <functional>
#include <vector>

#include "Voicemeeter/IChannel.h"

namespace Voicemeeter {
	namespace Remote {
		class Mixer;

		enum class LevelType : long {
			PreFaderInput = 0L,
			PostFaderInput = 1L,
			PostMuteInput = 2L,
			Output = 3L
		};

		class Channel : public IChannel {
		public:
			Channel(
				const Mixer& mixer,
				LevelType type,
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
			const Mixer& m_mixer;
			const LevelType m_type;
			const long m_channel;
			double m_level;
			::std::vector<::std::function<void(double)>> m_cCallback;
		};
	}
}