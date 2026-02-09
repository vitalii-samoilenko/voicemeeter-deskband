#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_NETWORK_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_NETWORK_HPP

#include <functional>
#include <unordered_map>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TNetwork>
			class Network : public TNetwork {
			public:
				using TToken = typename TNetwork::token;

				template<typename ...Args>
				inline explicit Network(Args &&...args)
					: TNetwork{ ::std::forward(args) ... }
					, _callbacks{} {

				};
				Network(Network const &) = delete;
				Network(Network &&) = delete;

				inline ~Network() = default;

				Network & operator=(Network const &) = delete;
				Network & operator=(Network &&) = delete;

				template<typename TClient>
				void set_Vban(bool value) {
					if (value == TNetwork::get_Vban()) {
						return;
					}
					TNetwork::set_Vban(value);
					for (auto &[clientId, callback] : _callbacks) {
						if (clientId == &typeid(TClient)) {
							continue;
						}
						callback(value);
					}
				};

				class token : public TToken {
				public:
					token() = delete;
					token(token const &) = delete;
					inline token(token &&) = default;

					inline ~token() {
						if (!TToken::clientId()) {
							return;
						}
						TToken::that<Network>()
							->_callbacks.erase(
								TToken::clientId());
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline void on_vban(Fn &&callback) {
						TToken::that<Network>()
							->_callbacks[TToken::clientId()]
								= ::std::forward<Fn>(callback);
					};

				protected:
					friend class Network;

					using TToken::TToken;
				};

				template<typename TClient>
				token Subscribe() {
					return token{ this, &typeid(TClient) };
				};

			private:
				friend class token;

				::std::unordered_map<
					void const *,
					::std::function<void(bool)>
				> _callbacks;

				using TNetwork::set_Vban;
			};
		}
	}
}

#endif
