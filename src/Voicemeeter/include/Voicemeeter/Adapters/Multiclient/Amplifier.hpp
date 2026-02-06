#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_AMPLIFIER_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_AMPLIFIER_HPP

#include <functional>
#include <unordered_map>
#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TAmplifier>
			class Amplifier : public TAmplifier {
			public:
				using TToken = typename TAmplifier::token;

				template<typename ...Args>
				inline explicit Amplifier(Args &&...args)
					: TAmplifier{ ::std::forward<Args>(args) ... }
					, _callbacks{} {

				};
				Amplifier(Amplifier const &) = delete;
				Amplifier(Amplifier &&) = delete;

				inline ~Amplifier() = default;

				Amplifier & operator=(Amplifier const &) = delete;
				Amplifier & operator=(Amplifier &&) = delete;

				template<typename TClient>
				void set_Gain(num_t value) {
					if (value == TAmplifier::get_Gain()) {
						return;
					}
					TAmplifier::set_Gain(value);
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
						TToken::that<Amplifier>()
							->_callbacks.erase(
								TToken::clientId());
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename Fn>
					inline void on_gain(Fn &&callback) {
						TToken::that<Amplifier>()
							->_callbacks[TToken::clientId()]
								= ::std::forward<Fn>(callback);
					};

				protected:
					friend class Amplifier;

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
					::std::function<void(num_t)>
				> _callbacks;

				using TAmplifier::set_Gain;
			};
		}
	}
}

#endif
