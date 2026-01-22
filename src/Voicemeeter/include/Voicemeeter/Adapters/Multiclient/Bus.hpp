#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_BUS_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_BUS_HPP

#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TBus>
			class Bus : public TBus {
			public:
				template<typename... Args>
				inline explicit Bus(Args &&...args)
					: TBus{ ::std::forward<Args>(args) ... } {

				};
				Bus(Bus const &) = delete;
				Bus(Bus &&) = delete;

				inline ~Bus() = default;

				Bus & operator=(Bus const &) = delete;
				Bus & operator=(Bus &&) = delete;

				class token {
				public:
					token() = delete;
					token(token const &) = delete;
					inline token(token &&other)
						: _clientId{ other._clientId } {
						other._clientId = nullptr;
					};

					inline ~token() = default;

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

				protected:
					void const *_clientId;

					inline token(
						void const *clientId,
						Bus &target)
						: _clientId{ clientId } {

					};
				};
			};
		}
	}
}

#endif
