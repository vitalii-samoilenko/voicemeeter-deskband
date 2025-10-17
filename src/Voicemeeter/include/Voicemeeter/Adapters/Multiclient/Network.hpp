#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_NETWORK_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_NETWORK_HPP

#include <functional>
#include <typeinfo>
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
					for (auto &[client, callback] : _callbacks) {
						if (&client == &typeid(TClient)) {
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
						: _client{ other._client }
						, _target{ other._target }{
						other._client = nullptr;
						other._target = nullptr;
					};

					inline ~token() {
						if (!_client || !_target) {
							return;
						}
						_target->_callbacks.erase(*_client);
						_client = nullptr;
						_target = nullptr;
					};

					token & operator=(token const &) = delete;
					inline token & operator=(token &&other) {
						_client = other._client;
						_target = other._target;
						other._client = nullptr;
						other._target = nullptr;
					};

					template<typename Fn>
					inline on_level(Fn &&callback) {
						_target->_callbacks[*_client] = ::std::forward<Fn>(callback);
					};

				private:
					::std::type_info const *_client;
					Network *_target;

					token(::std::type_info const &client, Network &target)
						: _client{ &client }
						, _target{ &target } {

					};

					friend Network;
				};

				template<typename TClient>
				token Subscribe() {
					return token{ typeid(TClient), *this };
				};

			private:
				::std::unordered_map<
					::std::type_info const &,
					::std::function<void(bool)>
				> _callbacks;

				using TNetwork::set_Vban;

				friend token;
			};
		}
	}
}

#endif
