#ifndef VOICEMEETER_ADAPTERS_MULTICLIENT_TOKEN_HPP
#define VOICEMEETER_ADAPTERS_MULTICLIENT_TOKEN_HPP

#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename T>
			class Token : public T {
			public:
				template<typename ...Args>
				inline explicit Token(Args &&...args)
					: T{ ::std::forward<Args>(args) ... } {

				};
				Token(Token const &) = delete;
				Token(Token &&) = delete;

				inline ~Token() = default;

				Token & operator=(Token const &) = delete;
				Token & operator=(Token &&) = delete;

				class token {
				public:
					token() = delete;
					token(token const &) = delete;
					inline token(token &&other)
						: _that{ other._that }
						, _clientId{ other._clientId } {
						other._clientId = nullptr;
					};

					inline ~token() = default;

					token & operator=(token const &) = delete;
					token & operator=(token &&) = delete;

				protected:
					inline token(
						void *that,
						void const *clientId)
						: _that{ that }
						, _clientId{ clientId } {

					};

					template<typename TThat>
					inline TThat * that() const {
						return reinterpret_cast<TThat *>(_that);
					};
					inline void const * clientId() const {
						return _clientId;
					};

				private:
					void *_that;
					void const *_clientId;
				};
			};
		}
	}
}

#endif
