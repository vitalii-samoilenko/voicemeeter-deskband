#ifndef VOICEMEETER_ADAPTERS_NETWORK_HPP
#define VOICEMEETER_ADAPTERS_NETWORK_HPP

#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		template<typename TMixer>
		class Network : public TMixer {
		public:
			template<typename... Args>
			inline explicit Network(Args &&...args)
				: TMixer{ ::std::forward<Args>(args)... }
				, _vban{ false } {

			};
			Network(Network const &) = delete;
			Network(Network &&) = delete;

			inline ~Network() = default;

			Network & operator=(Network const &) = delete;
			Network & operator=(Network &&) = delete;

			inline bool get_Vban() const {
				return _vban;
			};
			inline void set_Vban(bool value) {
				_vban = value;
			};

		private:
			bool _vban;
		};
	}
}

#endif
