#pragma once

#include <functional>
#include <vector>

#include "Voicemeeter/INetwork.h"

#include "Bus.h"

namespace Voicemeeter {
	namespace Remote {
		class Network : public INetwork {
		public:
			explicit Network(
				Mixer& mixer
			);
			Network() = delete;
			Network(const Network&) = delete;
			Network(Network&&) = delete;

			~Network() = default;

			Network& operator=(const Network&) = delete;
			Network& operator=(Network&&) = delete;

			virtual bool get_Vban() const override;
			virtual void set_Vban(bool value) override;
			void on_Vban(const ::std::function<void(bool)>& callback);

			void Update(bool dirty);

		private:
			Mixer& m_mixer;
			bool m_vban;
			::std::function<void(bool)> m_callback;
		};
	}
}