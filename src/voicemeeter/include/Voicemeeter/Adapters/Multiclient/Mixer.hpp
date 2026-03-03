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

				template<typename ...Args>
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
					typename TLayout::Strips From, typename TLayout::Strips To>
				inline void set_Plug(bool value) {
					if (value == TMixer::get_Plug<From, To>()) {
						return;
					}
					TMixer::set_Plug<From, To>(value);
					for (auto &[clientId, clientCallbacks] : _callbacks) {
						if (clientId == &typeid(TClient)) {
							continue;
						}
						::std::function<void(bool)> &callback{
							clientCallbacks[
								From * TLayout::OutputSize
								+ To - TLayout::InputSize
							]
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
					token(token const &) = delete;
					inline token(token &&) = default;

					inline ~token() {
						if (!TToken::clientId()) {
							return;
						}
						TToken::that<Mixer>()
							->_callbacks.erase(
								TToken::clientId());
					};

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

					template<typename TLayout::Strips From, typename TLayout::Strips To,
						typename Fn>
					inline void on_plug(Fn &&callback) {
						TToken::that<Mixer>()
							-> _callbacks[TToken::clientId()][
								From * TLayout::OutputSize
								+ To - TLayout::InputSize
							] = ::std::forward<Fn>(callback);
					};

				protected:
					friend class Mixer;

					using TToken::TToken;
				};

				template<typename TClient>
				inline token Subscribe() {
					return token{ this, &typeid(TClient) };
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
