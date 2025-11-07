#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_CHANNEL_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_CHANNEL_HPP

#include <functional>
#include <typeinfo>
#include <unordered_map>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TChannel>
			class Channel : public TChannel {
			public:
				template<typename... Args>
				inline explicit Channel(Args &&...args)
					: TChannel{ ::std::forward<Args>(args)... }
					, _callbacks{} {

				};
				Channel(Channel const &) = delete;
				Channel(Channel &&) = delete;

				inline ~Channel() = default;

				Channel & operator=(Channel const &) = delete;
				Channel & operator=(Channel &&) = delete;

				template<typename TClient>
				inline void set_Level(int value) {
					if (value == TChannel::get_Level()) {
						return;
					}
					TChannel::set_Level(value);
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
					Channel *_target;

					token(::std::type_info const &client, Channel &target)
						: _client{ &client }
						, _target{ &target } {

					};

					friend Channel;
				};

				template<typename TClient>
				token Subscribe() {
					return token{ typeid(TClient), *this };
				};

			private:
				::std::unordered_map<
					::std::type_info const &,
					::std::function<void(int)>
				> _callbacks;

				using TChannel::set_Level;

				friend token;
			};
		}
	}
}

#endif
