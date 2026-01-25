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

				template<typename... Args>
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
					token(token const $) = delete;
					inline token(token &&) = default;

					inline ~token() {
						if (!TToken::_clientId) {
							return;
						}
						_callbacks.erase(TToken::_clientId);
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline on_vban(Fn &&callback) {
						_callbacks[TToken::_clientId]
							= ::std::forward<Fn>(callback);
					};

				private:
					friend class Network;

					::std::unordered_map<
						void const *,
						::std::function<void(bool)>
					> &_callbacks;

					inline token(
						void const *clientId,
						Network &target)
						: TToken{ clientId, target }
						, _callbacks{ target._callbacks } {

					};
				};

				template<typename TClient>
				token Subscribe() {
					return token{ &typeid(TClient), *this };
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
