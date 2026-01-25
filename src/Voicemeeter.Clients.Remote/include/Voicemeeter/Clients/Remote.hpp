#ifndef VOICEMEETER_CLIENTS_REMOTE_HPP
#define VOICEMEETER_CLIENTS_REMOTE_HPP

#include <exception>
#include <memory>
#include <utility>

#include "vmr_client.h"
#include "Voicemeeter/Clients/_/Remote.hpp"
#include "VoicemeeterRemote.h"

namespace Voicemeeter {
	namespace Clients {
		template<
			typename TTimer,
			typename TMixer>
		class RemoteBuilder;

		template<
			typename TTimer,
			typename TMixer>
		class Remote final {
		public:
			using Type = _::Remote::runtime_t;

			Remote() = delete;
			Remote(Remote const &) = delete;
			Remote(Remote &&) = delete;

			inline ~Remote() {
				_client.VBVMR_Logout();
			};

			Remote & operator=(Remote const &) = delete;
			Remote & operator=(Remote &&) = delete;

			inline Type get_Type() const {
				runtime_t value{ 0L };
				if (_client.VBVMR_GetVoicemeeterType(&value)) {
					throw ::std::exception{ "Could not get Voicemeeter type" };
				}
				return value;
			};

		private:
			friend class RemoteBuilder<TTimer, TMixer>;

			class RemoteTick final {
			public:
				inline RemoteTick(
					TTimer &timer,
					TMixer &mixer,
					T_VBVMR_INTERFACE &client)
					: _timer{ timer }
					, _mixer{ mixer }
					, _client{ client } {

				};
				RemoteTick() = delete;
				RemoteTick(RemoteTick const &) = delete;
				RemoteTick(RemoteTick &&) = delete;

				inline ~RemoteTick() {
					Unset();
				};

				RemoteTick & operator=(RemoteTick const &) = delete;
				RemoteTick & operator=(RemoteTick &&) = delete;

				inline void operator()() const {
					_::Remote::Update(_mixer, _client);
				};

				inline void Set() {
					_timer.Set(100, *this);
				};
				inline void Unset() {
					_timer.Unset(*this);
				};

			private:
				TTimer &_timer;
				TMixer &_mixer;
				T_VBVMR_INTERFACE &_client;
			};

			T_VBVMR_INTERFACE _client;
			RemoteTick _remoteTick;
			_::Remote::bag<TMixer> _tokens;

			inline Remote(
				T_VBVMR_INTERFACE &&client,
				TTimer &timer,
				TMixer &mixer)
				: _client{ ::std::move(client) }
				, _remoteTick{ timer, mixer, _client }
				, _tokens{ _::Remote::Subscribe(mixer, _client) } {
				_remoteTick.Set();
			};
		};

		template<
			typename TTimer,
			typename TMixer>
		class RemoteBuilder final {
		public:
			inline RemoteBuilder(
				TTimer &timer,
				TMixer &mixer)
				: _timer{ timer }
				, _mixer{ mixer } {

			};
			RemoteBuilder() = delete;
			RemoteBuilder(RemoteBuilder const &) = delete;
			RemoteBuilder(RemoteBuilder &&) = delete;

			inline ~RemoteBuilder() = default;

			RemoteBuilder & operator=(RemoteBuilder const &) = delete;
			RemoteBuilder & operator=(Remote &&) = delete;

			::std::unique_ptr<Remote<TTimer, TMixer>> Build() const {
				T_VBVMR_INTERFACE client{};
				if (::InitializeDLLInterfaces(client) != 0) {
					throw ::std::exception{ "Could not initialize interfaces" };
				}
				long login{ client.VBVMR_Login() };
				if (login < 0) {
					throw ::std::exception{ "Could not connect to Voicemeeter" };
				} else if (login) {
					client.VBVMR_Logout();
					throw ::std::exception{ "Voicemeeter is not started" };
				}
				return ::std::make_unique<Remote<TTimer, TMixer>>(
					::std::move(client), _timer, _mixer);
			};

		private:
			TTimer &_timer;
			TMixer &_mixer;
		};
	}
}

#endif
