#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_MIXER_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_MIXER_HPP

#include <array>
#include <functional>
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
					for (auto &[clientId, clientCallbacks] : _callbacks) {
						if (clientId == &typeid(TClient)) {
							continue;
						}
						::std::function<void(bool)> &callback{ clientCallbacks[TMixer::OutputSize * i + j] };
						if (!callback) {
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
						_callbacks.erase(_clientId);
						_clientId = nullptr;
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename TIStrip, typename TOStrip, typename Fn>
					inline on_plug(TIStrip const &input, TOStrip const &output, Fn &&callback) {
						size_t i{ _target->get_Index(input) };
						size_t j{ _target->get_Index(output) };
						_callbacks[_clientId][TMixer::OutputSize * i + j] = ::std::forward<Fn>(callback);
					};

				private:
					friend Mixer;

					void const *_clientId;
				::std::unordered_map<
					void const *,
					::std::array<
						::std::function<void(bool)>,
						TMixer::InputSize
						* TMixer::OutputSize
				>> &_callbacks;

					token(
						void const *clientId,
						::std::unordered_map<
							void const *,
							::std::array<
								::std::function<void(bool)>,
								TMixer::InputSize
								* TMixer::OutputSize
						>> &callbacks)
						: _clientId{ clientId }
						, _callbacks{ callbacks } {

					};
				};

				template<typename TClient>
				inline token Subscribe() {
					return token{ &typeid(TClient), _callbacks };
				};

			private:
				::std::unordered_map<
					void const *,
					::std::array<
						::std::function<void(bool)>,
						TMixer::InputSize
						* TMixer::OutputSize
				>> _callbacks;

				using TMixer::set_Plug;
			};
		}
	}
}

#endif
