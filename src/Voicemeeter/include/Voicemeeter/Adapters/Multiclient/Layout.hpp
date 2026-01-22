#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_LAYOUT_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_LAYOUT_HPP

#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename TLayout>
			class Layout : public TLayout {
			public:
				template<typename... Args>
				inline explicit Layout(Args &&...args)
					: TLayout{ ::std::forward<Args>(args) ... } {

				};
				Layout(Layout const &) = delete;
				Layout(Layout &&) = delete;

				inline ~Layout() = default;

				Layout & operator=(Layout const &) = delete;
				Layout & operator=(Layout &&) = delete;

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
						Layout &target)
						: _clientId{ clientId } {

					};
				};
			};
		}
	}
}

#endif
