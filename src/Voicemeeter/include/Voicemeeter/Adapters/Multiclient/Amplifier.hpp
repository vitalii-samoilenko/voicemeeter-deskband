#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_AMPLIFIER_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_AMPLIFIER_HPP

#include <functional>
#include <unordered_map>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TAmplifier>
			class Amplifier : public TAmplifier {
			public:
				template<typename... Args>
				inline explicit Amplifier(Args &&...args)
					: TAmplifier{ ::std::forward<Args>(args)... }
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

				class token {
				public:
					token() = delete;
					token(token const $) = delete;
					inline token(token &&other)
						: _clientId{ other._clientId }
						, _callbacks{ other._target }{
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
					inline on_gain(Fn &&callback) {
						_callbacks[_clientId] = ::std::forward<Fn>(callback);
					};

				private:
					friend Amplifier;

					void const *_clientId;
					::std::unordered_map<
						void const *,
						::std::function<void(num_t)>
					> &_callbacks;

					token(
						void const *clientId,
						::std::unordered_map<
							void const *,
							::std::function<void(num_t)>
						> &callbacks)
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
					::std::function<void(num_t)>
				> _callbacks;

				using TAmplifier::set_Gain;
			};
		}
	}
}

#endif
