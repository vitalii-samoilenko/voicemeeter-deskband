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
				return _runtime;
				return value;
			};

		private:
			friend class RemoteBuilder<TTimer, TMixer>;

			class RemoteTick final {
			public:
				inline RemoteTick(
					TTimer &timer,
					TMixer &mixer,
					T_VBVMR_INTERFACE &client,
					_::Remote::runtime_t runtime)
					: _timer{ timer }
					, _mixer{ mixer }
					, _client{ client }
					, _runtime{ runtime } {

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
					_::Remote::Update(_mixer, _client, _runtime);
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
				_::Remote::runtime_t _runtime;
			};

			T_VBVMR_INTERFACE _client;
			_::Remote::runtime_t _runtime;
			RemoteTick _remoteTick;
			_::Remote::bag<TMixer> _tokens;

			inline Remote(
				T_VBVMR_INTERFACE &&client,
				_::Remote::runtime_t runtime,
				TTimer &timer,
				TMixer &mixer)
				: _client{ ::std::move(client) }
				, _runtime{ runtime }
				, _remoteTick{ timer, mixer, _client }
				, _tokens{ _::Remote::Subscribe(mixer, _client, _runtime) } {
				_remoteTick.Set();
			};
		};

		template<
			typename TTimer,
			typename TMixer>
		class RemoteBuilder final {
		public:
			using Remote = Remote<TTimer, TMixer>;

			inline RemoteBuilder() = default;
			RemoteBuilder(RemoteBuilder const &) = delete;
			RemoteBuilder(RemoteBuilder &&) = delete;

			inline ~RemoteBuilder() = default;

			RemoteBuilder & operator=(RemoteBuilder const &) = delete;
			RemoteBuilder & operator=(Remote &&) = delete;

			inline void set_Timer(TTimer &value) {
				_timer = &value;
			};
			inline void set_Mixer(TMixer &value) {
				_mixer = &value;
			};
			inline void set_Type(Remote::Type value) {
				_runtime = value;
			};

			::std::unique_ptr<Remote> Build() const {
				if (!_timer) {
					throw ::std::exception{ "Timer is not set" };
				}
				if (!_mixer) {
					throw ::std::exception{ "Mixer is not set" };
				}
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
				_::Remote::runtime_t runtime{ 0L };
				if (client.VBVMR_GetVoicemeeterType(&runtime)) {
					client.VBVMR_Logout();
					throw ::std::exception{ "Could not get Voicemeeter type" };
				}
				if (_runtime < runtime) {
					runtime = _runtime;
				}
				return ::std::make_unique<Remote>(
					::std::move(client), runtime, *_timer, *_mixer);
			};

		private:
			TTimer *_timer;
			TMixer *_mixer;
			_::Remote::runtime_t _runtime;
		};
	}
}

#endif
