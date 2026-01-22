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
				using TToken = typename TMixer::token;
				using TLayout = Mixer;

				template<typename... Args>
				inline explicit Mixer(Args &&...args)
					: TMixer{ ::std::forward<Args>(args) ... }
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
					size_t i{ TLayout::get_Index(input) };
					size_t j{ TLayout::get_Index(output) };
					for (auto &[clientId, clientCallbacks] : _callbacks) {
						if (clientId == &typeid(TClient)) {
							continue;
						}
						::std::function<void(bool)> &callback{
							clientCallbacks[TLayout::OutputSize * i + j]
						};
						if (!callback) {
							continue;
						}
						callback(value);
					}
				};

				class token : public TToken {
				public:
					token() = delete;
					token(token const $) = delete;
					inline token(token &&) = default;

					inline ~token() {
						if (!TToken::_clientId) {
							return;
						}
						_callbacks.erase(TToken::_clientId);
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename TIStrip, typename TOStrip, typename Fn>
					inline on_plug(TIStrip const &input, TOStrip const &output, Fn &&callback) {
						size_t i{ _layout.get_Index(input) };
						size_t j{ _layout.get_Index(output) };
						_callbacks[TToken::_clientId][TLayout::OutputSize * i + j]
							= ::std::forward<Fn>(callback);
					};

				private:
					friend class Mixer;

					TLayout &_layout;
					::std::unordered_map<
						void const *,
						::std::array<
							::std::function<void(bool)>,
							TLayout::InputSize
							* TLayout::OutputSize
					>> &_callbacks;

					inline token(
						void const *clientId,
						Mixer &target)
						: TToken{ clientId, target }
						, _layout{ target }
						, _callbacks{ callbacks } {

					};
				};

				template<typename TClient>
				inline token Subscribe() {
					return token{ &typeid(TClient), *this };
				};

			private:
				friend class token;

				::std::unordered_map<
					void const *,
					::std::array<
						::std::function<void(bool)>,
						TLayout::InputSize
						* TLayout::OutputSize
				>> _callbacks;

				using TMixer::set_Plug;
			};
		}
	}
}

#endif
