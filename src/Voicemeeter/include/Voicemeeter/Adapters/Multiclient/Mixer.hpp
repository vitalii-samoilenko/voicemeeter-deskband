#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_MIXER_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_MIXER_HPP

#include <array>
#include <functional>
#include <typeinfo>
#include <unordered_map>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TMixer>
			class Mixer : public TMixer {
			public:
				template<typename... Args>
				inline explicit Mixer(Args &&...args)
					: TMixer{ ::std::forward<Args>(args)... }
					, _callbacks{} {

				};
				Mixer(Mixer const &) = delete;
				Mixer(Mixer &&) = delete;

				inline ~Mixer() = default;

				Mixer & operator=(Mixer const &) = delete;
				Mixer & operator=(Mixer &&) = delete;

				template<typename TClient,
					typename TIStrip, typename TOStrip>
				inline void set_Plug(TIStrip const &input, TOStrip const &output, bool value) {
					if (value == TMixer::get_Plug(input, output)) {
						return;
					}
					TMixer::set_Plug(input, output);
					size_t i{ TMixer::get_Index(input) };
					size_t j{ TMixer::get_Index(output) };
					for (auto &[client, callbacks] : _callbacks) {
						if (&client == &typeid(TClient)) {
							continue;
						}
						::std::function<void(bool>) &callback{ callbacks[TMixer::OutputSize * i + j] };
						if (callback) {
							callback(value);
						}
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

					template<typename TIStrip, typename TOStrip, typename Fn>
					inline on_plug(TIStrip const &input, TOStrip const &output, Fn &&callback) {
						size_t i{ _target->get_Index(input) };
						size_t j{ _target->get_Index(output) };
						_target->_callbacks[*_client][TMixer::OutputSize * i + j] = ::std::forward<Fn>(callback);
					};

				private:
					::std::type_info const *_client;
					Mixer *_target;

					token(::std::type_info const &client, Mixer &target)
						: _client{ &client }
						, _target{ &target } {

					};

					friend Mixer;
				};

				template<typename TClient>
				inline token Subscribe() {
					return token{ typeid(TClient), *this };
				};

			private:
				::std::unordered_map<
					::std::type_info const &,
					::std::array<
						::std::function<void(bool)>,
						TMixer::InputSize
						* TMixer::OutputSize
				>> _callbacks;

				using TMixer::set_Plug;

				friend token;
			};
		}
	}
}

#endif
