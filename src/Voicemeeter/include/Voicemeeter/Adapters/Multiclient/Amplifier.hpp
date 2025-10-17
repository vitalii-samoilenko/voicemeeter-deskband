#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_AMPLIFIER_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_AMPLIFIER_HPP

#include <cmath>
#include <functional>
#include <typeinfo>
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
				void set_Gain(double value) {
					if (::std::abs(value - TAmplifier::get_Gain()) < 0.01) {
						return;
					}
					TAmplifier::set_Gain(value);
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
					inline on_gain(Fn &&callback) {
						_target->_callbacks[*_client] = ::std::forward<Fn>(callback);
					};

				private:
					::std::type_info const *_client;
					Amplifier *_target;

					token(::std::type_info const &client, Amplifier &target)
						: _client{ &client }
						, _target{ &target } {

					};

					friend Amplifier;
				};

				template<typename TClient>
				token Subscribe() {
					return token{ typeid(TClient), *this };
				};

			private:
				::std::unordered_map<
					::std::type_info const &,
					::std::function<void(double)>
				> _callbacks;

				using TAmplifier::set_Gain;

				friend token;
			};
		}
	}
}

#endif
