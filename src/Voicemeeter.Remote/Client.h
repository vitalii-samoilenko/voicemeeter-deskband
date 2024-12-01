#pragma once

#include <exception>
#include <string>
#include <type_traits>

#include "Environment/ITimer.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"

#include "Api.h"

namespace Voicemeeter {
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
				Subscribe(timer);
			};
			Client() = delete;
			Client(const Client&) = delete;
			Client(Client&&) = delete;

			inline ~Client() {
				m_remote.VBVMR_Logout();
			};

			Client& operator=(const Client&) = delete;
			Client& operator=(Client&&) = delete;

		private:
			TMixer& m_mixer;
			T_VBVMR_INTERFACE m_remote;
			Type m_type;

			void Subscribe(::Environment::ITimer& timer) const;

			template<Type Remote>
			static ::std::string ToBusKey(size_t id);
			template<Type Remote>
			static long ToChannelKey(size_t id);

			inline ::std::string ToBusKey(size_t id) const {
				switch (m_type) {
				case Type::Voicemeeter:
					return ToBusKey<Type::Voicemeeter>(id);
				case Type::Banana:
					return ToBusKey<Type::Banana>(id);
				case Type::Potato:
					return ToBusKey<Type::Potato>(id);
				default:
					throw ::std::exception{ "Remote type is not supported" };
				}
			};
			inline long ToChannelKey(size_t id) const {
				switch (m_type) {
				case Type::Voicemeeter:
					return ToChannelKey<Type::Voicemeeter>(id);
				case Type::Banana:
					return ToChannelKey<Type::Banana>(id);
				case Type::Potato:
					return ToChannelKey<Type::Potato>(id);
				default:
					throw ::std::exception{ "Remote type is not supported" };
				}
			};

			template<typename TBus>
			void Update(TBus& bus, size_t size, LevelType levelType, bool dirty) const {
				size_t i{ 0 };
				float value{ 0.F };
				for (auto& strip : bus) {
					if (dirty) {
						::std::string key{ ToBusKey(strip.get_Id()) };
						m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Gain").c_str()), &value);
						strip.set_Gain<Client>(value);
						m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Mute").c_str()), &value);
						strip.set_Mute<Client>(0.01F < value);
					}
					for (auto& line : strip) {
						long key{ ToChannelKey(line.get_Id()) };
						m_remote.VBVMR_GetLevel(static_cast<long>(levelType), key, &value);
						line.set_Level<Client>(value);
					}
					if (++i == size) {
						break;
					}
				}
			};
		};
	}
}