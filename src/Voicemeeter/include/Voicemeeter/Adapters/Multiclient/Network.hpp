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
				template<typename... Args>
				inline explicit Network(Args &&...args)
					: TNetwork{ ::std::forward(args)... }
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

				class token {
				public:
					token() = delete;
					token(token const $) = delete;
					inline token(token &&other)
						: _clientId{ other._clientId }
						, _callbacks{ other._callbacks }{
						other._clientId = nullptr;
					};

					inline ~token() {
						if (!_clientId) {
							return;
						}
						_callbacks.erase(_clientId);
						_clientId = nullptr;
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline on_level(Fn &&callback) {
						_callbacks[_clientId] = ::std::forward<Fn>(callback);
					};

				private:
					friend Network;

					void const *_clientId;
				::std::unordered_map<
					void const *,
					::std::function<void(bool)>
				> &_callbacks;

					token(
						void const *clientId,
						::std::unordered_map<
							void const *,
							::std::function<void(bool)>
						> &callbacks
						: _clientId{ clientId }
						, _callbacks{ callbacks } {

					};
				};

				template<typename TClient>
				token Subscribe() {
					return token{ &typeid(TClient), _callbacks };
				};

			private:
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
