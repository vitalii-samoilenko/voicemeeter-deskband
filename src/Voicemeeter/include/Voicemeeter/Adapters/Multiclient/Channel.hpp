#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_CHANNEL_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_CHANNEL_HPP

#include <functional>
#include <unordered_map>
#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TChannel>
			class Channel : public TChannel {
			public:
				template<typename... Args>
				inline explicit Channel(Args &&...args)
					: TChannel{ ::std::forward<Args>(args) ... }
					, _callbacks{} {

				};
				Channel(Channel const &) = delete;
				Channel(Channel &&) = delete;

				inline ~Channel() = default;

				Channel & operator=(Channel const &) = delete;
				Channel & operator=(Channel &&) = delete;

				template<typename TClient>
				inline void set_Level(num_t value) {
					if (value == TChannel::get_Level()) {
						return;
					}
					TChannel::set_Level(value);
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
						callbacks.erase(_clientId);
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline on_level(Fn &&callback) {
						_callbacks[_clientId]
							= ::std::forward<Fn>(callback);
					};

				protected:
					void const *_clientId;

					inline token(
						void const *clientId,
						Channel &target)
						: _clientId{ clientId }
						, _callbacks{ target._callbacks } {

					};

				private:
					friend class Channel;

					::std::unordered_map<
						void const *,
						::std::function<void(num_t)>
					> &_callbacks;
				};

				template<typename TClient>
				token Subscribe() {
					return token{ &typeid(TClient), *this };
				};

			private:
				friend class token;

				::std::unordered_map<
					void const *,
					::std::function<void(num_t)>
				> _callbacks;

				using TChannel::set_Level;
			};
		}
	}
}

#endif
