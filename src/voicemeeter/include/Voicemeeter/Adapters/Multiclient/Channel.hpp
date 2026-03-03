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
				using TToken = typename TChannel::token;

				template<typename ...Args>
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

				class token : public TToken {
				public:
					token() = delete;
					token(token const &) = delete;
					inline token(token &&) = default;

					inline ~token() {
						if (!TToken::clientId()) {
							return;
						}
						TToken::that<Channel>()
							->_callbacks.erase(
								TToken::clientId());
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline void on_level(Fn &&callback) {
						TToken::that<Channel>()
							->_callbacks[TToken::clientId()]
								= ::std::forward<Fn>(callback);
					};

				protected:
					friend class Channel;

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
					::std::function<void(num_t)>
				> _callbacks;

				using TChannel::set_Level;
			};
		}
	}
}

#endif
