#ifndef VOICEMEETER_CLIENTS_REMOTE_HPP
#define VOICEMEETER_CLIENTS_REMOTE_HPP

#include <exception>
#include <memory>

#include "VoicemeeterRemoteLoader.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"

namespace Voicemeeter {
	namespace Clients {
		template<
			typename TMixer,
			typename TTimer>
		class Remote final {
		public:
			enum class LevelType : long {
				PreFaderInput = 0L,
				PostFaderInput = 1L,
				PostMuteInput = 2L,
				Output = 3L
			};

			inline Remote(
				TMixer &mixer,
				TTimer &timer)
				: _mixer{ mixer }
				, _timer{ timer } {

			};
			Remote() = delete;
			Remote(Remote const &) = delete;
			Remote(Remote &&) = delete;

			inline ~Remote() = default;

			Remote & operator=(Remote const &) = delete;
			Remote & operator=(Remote &&) = delete;

			class vbvmr final {
			public:
				vbvmr() = delete;
				vbvmr(vbvmr const &) = delete;
				vbvmr(vbvmr &&) = delete;

				inline ~vbvmr() {
					_remote.VBVMR_Logout();
				};

				vbvmr & operator=(vbvmr const &) = delete;
				vbvmr & operator=(vbvmr &&) = delete;

				enum class Type : long {
					Voicemeeter = 1L,
					Banana = 2L,
					Potato = 3L
				};
				inline Type get_Type() const {
					return _type;
				};

			private:
				class RemoteTick final {
				public:
					inline RemoteTick(
						TMixer &mixer,
						TTimer &timer,
						T_VBVMR_INTERFACE &remote)
						: _mixer{ mixer }
						, _timer{ timer }
						, _remote{ remote } {

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
						_::Update(_mixer, _remote);
					};

					inline void Set() {
						_timer.Set(100, *this);
					};
					inline void Unset() {
						_timer.Unset(*this);
					};

				private:
					TMixer &_mixer;
					TTimer &_timer;
					T_VBVMR_INTERFACE &_remote;
				};

				friend Remote;

				T_VBVMR_INTERFACE _remote;
				Type _type;
				RemoteTick _remoteTick;
				_::Bag<TMixer> _tokens;

				inline vbvmr(
					TMixer &mixer,
					TTimer &timer)
					: _remote{}
					, _type{ Type::Voicemeeter }
					, _remoteTick{ mixer, timer, _remote }
					, _tokens{ mixer } {
					if (::InitializeDLLInterfaces(_remote) != 0) {
						throw ::std::exception{ "Cannot initialize interfaces" };
					}
					long login{ _remote.VBVMR_Login() };
					if (login < 0) {
						throw ::std::exception{ "Cannot connect to Voicemeeter" };
					} else if (login) {
						_remote.VBVMR_Logout();
						throw ::std::exception{ "Voicemeeter is not started" };
					} if (_remote.VBVMR_GetVoicemeeterType(reinterpret_cast<long *>(&_type))) {
						_remote.VBVMR_Logout();
						throw ::std::exception{ "Cannot get Voicemeeter type" };
					}
					Subscribe();
					_remoteTick.Set();
				};
			};

			::std::unique_ptr<vbvmr> Build() const {
				return ::std::make_unique<vbvmr>(
					_mixer, timer);
			};

		private:
			TMixer &_mixer;
			TTimer &_timer;
		};
	}
}

#endif
