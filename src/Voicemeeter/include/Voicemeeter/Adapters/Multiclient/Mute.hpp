#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_MUTE_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_MUTE_HPP

#include <functional>
#include <unordered_map>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TMute>
			class Mute : public TMute {
			public:
				using TToken = typename TMute::token;

				template<typename ...Args>
				inline explicit Mute(Args &&...args)
					: TMute{ ::std::forward<Args>(args) ... }
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
						TToken::that<Mute>()
							->_callbacks.erase(
								TToken::clientId());
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline void on_mute(Fn &&callback) {
						TToken::that<Mute>()
							->_callbacks[TToken::clientId()]
								= ::std::forward<Fn>(callback);
					};

				protected:
					friend class Mute;

					using TToken::TToken;
				};

				template<typename TClient>
				token Subscribe() {
					return token{ &typeid(TClient), this };
				};

			private:
				friend class token;

				::std::unordered_map<
					void const *,
					::std::function<void(bool)>
				> _callbacks;

				using TMute::set_Mute;
			};
		}
	}
}

#endif
