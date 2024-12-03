#pragma once

#include <exception>
#include <string>
#include <type_traits>

#include "Environment/ITimer.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"

#include "Api.h"

namespace Voicemeeter {
	namespace Clients {
		namespace Remote {
			enum class Type : long {
				Voicemeeter = 1,
				Banana = 2,
				Potato = 3
			};
			enum class LevelType : long {
				PreFaderInput = 0L,
				PostFaderInput = 1L,
				PostMuteInput = 2L,
				Output = 3L
			};

			template<typename TMixer>
			class Client {
				static_assert(
					::std::is_same_v<Adapters::Multiclient::Cherry, TMixer>,
					"TMixer is not supported");

			public:
				Client(
					::Environment::ITimer& timer,
					TMixer& mixer
				) : m_mixer{ mixer }
				  , m_remote{}
				  , m_type{ Type::Voicemeeter } {
					if (InitializeDLLInterfaces(m_remote) != 0) {
						throw ::std::exception{ "Cannot initialize interfaces" };
					}
					long login{ m_remote.VBVMR_Login() };
					if (login < 0) {
						throw ::std::exception{ "Cannot connect to Voicemeeter" };
					} else if (login) {
						m_remote.VBVMR_Logout();
						throw ::std::exception{ "Voicemeeter is not started" };
					} if (m_remote.VBVMR_GetVoicemeeterType(reinterpret_cast<long*>(&m_type))) {
						m_remote.VBVMR_Logout();
						throw ::std::exception{ "Cannot get Voicemeeter type" };
					}
					Subscribe();
					timer.Set(::std::chrono::milliseconds{ 100 },
						[this]()->bool {
							Update();
							return true;
						});
				};
				Client() = delete;
				Client(const Client&) = delete;
				Client(Client&&) = delete;

				inline ~Client() {
					m_remote.VBVMR_Logout();
				};

				Client& operator=(const Client&) = delete;
				Client& operator=(Client&&) = delete;

				inline Type get_Type() const {
					return m_type;
				};

			private:
				TMixer& m_mixer;
				T_VBVMR_INTERFACE m_remote;
				Type m_type;

				inline void Subscribe() const;
				inline void Update() const;
			};
		}
	}
}