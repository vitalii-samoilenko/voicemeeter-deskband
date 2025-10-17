#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_MUTE_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_MUTE_HPP

#include <functional>
#include <typeinfo>
#include <unordered_map>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TMute>
			class Mute : public TMute {
			public:
				template<typename... Args>
				inline explicit Mute(Args &&...args)
					: TMute{ ::std::forward<Args>(args)... }
					, _callbacks{} {

				};
				Mute(Mute const &) = delete;
				Mute(Mute &&) = delete;

				inline ~Mute() = default;

				Mute & operator=(Mute const &) = delete;
				Mute & operator=(Mute &&) = delete;

				template<typename TClient>
				void set_Mute(bool value) {
					if (value == TMute::get_Mute()) {
						return;
					}
					TMute::set_Mute(value);
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
					inline on_mute(Fn &&callback) {
						_target->_callbacks[*_client] = ::std::forward<Fn>(callback);
					};

				private:
					::std::type_info const *_client;
					Mute *_target;

					token(::std::type_info const &client, Mute &target)
						: _client{ &client }
						, _target{ &target } {

					};

					friend Mute;
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

				using TMute::set_Mute;

				friend token;
			};
		}
	}
}

#endif
