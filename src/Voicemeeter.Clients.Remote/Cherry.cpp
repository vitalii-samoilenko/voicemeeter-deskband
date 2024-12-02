#include "Client.h"

using namespace ::Voicemeeter::Clients::Remote;

template<>
template<>
::std::string Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToBusKey<Type::Voicemeeter>(size_t id) {
	switch (id) {
	case 0:
		return "Strip[0]";
	case 1:
		return "Strip[2]";
	case 2:
		return "Bus[0]";
	case 3:
		throw ::std::exception{ "Strip 3 is not supported" };
	case 4:
		return "Bus[1]";
	case 5:
		throw ::std::exception{ "Strip 5 is not supported" };
	default:
		throw ::std::exception{ "Strip is not supported" };
	}
}
template<>
template<>
::std::string Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToBusKey<Type::Banana>(size_t id) {
	switch (id) {
	case 0:
		return "Strip[0]";
	case 1:
		return "Strip[3]";
	case 2:
		return "Bus[0]";
	case 3:
		return "Bus[1]";
	case 4:
		return "Bus[3]";
	case 5:
		return "Bus[4]";
	default:
		throw ::std::exception{ "Strip is not supported" };
	}
}
template<>
template<>
::std::string Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToBusKey<Type::Potato>(size_t id) {
	switch (id) {
	case 0:
		return "Strip[0]";
	case 1:
		return "Strip[5]";
	case 2:
		return "Bus[0]";
	case 3:
		return "Bus[1]";
	case 4:
		return "Bus[5]";
	case 5:
		return "Bus[6]";
	default:
		throw ::std::exception{ "Strip is not supported" };
	}
}


template<>
::std::string Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToBusSubKey(size_t id) {
	switch (id) {
	case 0:
		throw ::std::exception{ "Strip 0 sub key is not supported" };
	case 1:
		throw ::std::exception{ "Strip 1 sub key is not supported" };
	case 2:
		return "A1";
	case 3:
		return "A2";
	case 4:
		return "B1";
	case 5:
		return "B2";
	default:
		throw ::std::exception{ "Strip sub key is not supported" };
	}
}

template<>
template<>
long Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToChannelKey<Type::Voicemeeter>(size_t id) {
	if (id < 2) {
		return static_cast<long>(id);
	} else if (id < 10) {
		return static_cast<long>(id) + 2L;
	} else if (id < 18) {
		return static_cast<long>(id) - 10L;
	} else if (id < 26) {
		throw ::std::exception{ "Strip 3 is not supported" };
	} else if (id < 34) {
		return static_cast<long>(id) - 18L;
	} else {
		throw ::std::exception{ "Strip 5 is not supported" };
	}
}
template<>
template<>
long Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToChannelKey<Type::Banana>(size_t id) {
	if (id < 2) {
		return static_cast<long>(id);
	} else if (id < 10) {
		return static_cast<long>(id) + 4L;
	} else if (id < 26) {
		return static_cast<long>(id) - 10L;
	} else {
		return static_cast<long>(id) - 2L;
	}
}
template<>
template<>
long Client<::Voicemeeter::Adapters::Multiclient::Cherry>::ToChannelKey<Type::Potato>(size_t id) {
	if (id < 2) {
		return static_cast<long>(id);
	} else if (id < 10) {
		return static_cast<long>(id) + 8L;
	} else if (id < 26) {
		return static_cast<long>(id) - 10L;
	} else {
		return static_cast<long>(id) + 14L;
	}
}

void Client<::Voicemeeter::Adapters::Multiclient::Cherry>::Subscribe(::Environment::ITimer& timer) const {
	auto& subscription = m_mixer.get_Subscription<Client>();
	if (Type::Voicemeeter < m_type) {
		subscription.on_Vban(
			[this](bool value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>("vban.Enable"), static_cast<float>(value));
			});
	}
	for (auto& input : m_mixer.get_PhysicalInput()) {
		size_t inputId{ input.get_Id() };
		::std::string inputKey{ ToBusKey(inputId) };
		subscription.on_Gain(inputId,
			[this, key = inputKey + ".Gain"](double value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		subscription.on_Mute(inputId,
			[this, key = inputKey + ".Mute"](bool value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		for (auto& output : m_mixer.get_PhysicalOutput()) {
			size_t outputId{ output.get_Id() };
			::std::string outputKey{ ToBusSubKey(outputId) };
			subscription.on_Plug(inputId, outputId,
				[this, key = inputKey + "." + outputKey](bool value)->void {
					m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
				});
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
		for (auto& output : m_mixer.get_VirtualOutput()) {
			size_t outputId{ output.get_Id() };
			::std::string outputKey{ ToBusSubKey(outputId) };
			subscription.on_Plug(inputId, outputId,
				[this, key = inputKey + "." + outputKey](bool value)->void {
					m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
				});
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
	}
	for (auto& input : m_mixer.get_VirtualInput()) {
		size_t inputId{ input.get_Id() };
		::std::string inputKey{ ToBusKey(inputId) };
		subscription.on_Gain(inputId,
			[this, key = inputKey + ".Gain"](double value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		subscription.on_Mute(inputId,
			[this, key = inputKey + ".Mute"](bool value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		for (auto& output : m_mixer.get_PhysicalOutput()) {
			size_t outputId{ output.get_Id() };
			::std::string outputKey{ ToBusSubKey(outputId) };
			subscription.on_Plug(inputId, outputId,
				[this, key = inputKey + "." + outputKey](bool value)->void {
					m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
				});
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
		for (auto& output : m_mixer.get_VirtualOutput()) {
			size_t outputId{ output.get_Id() };
			::std::string outputKey{ ToBusSubKey(outputId) };
			subscription.on_Plug(inputId, outputId,
				[this, key = inputKey + "." + outputKey](bool value)->void {
					m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
				});
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
	}
	for (auto& output : m_mixer.get_PhysicalOutput()) {
		size_t outputId{ output.get_Id() };
		::std::string outputKey{ ToBusKey(outputId) };
		subscription.on_Gain(outputId,
			[this, key = outputKey + ".Gain"](double value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		subscription.on_Mute(outputId,
			[this, key = outputKey + ".Mute"](bool value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		if (m_type == Type::Voicemeeter) {
			break;
		}
	}
	for (auto& output : m_mixer.get_VirtualOutput()) {
		size_t outputId{ output.get_Id() };
		::std::string outputKey{ ToBusKey(outputId) };
		subscription.on_Gain(outputId,
			[this, key = outputKey + ".Gain"](double value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		subscription.on_Mute(outputId,
			[this, key = outputKey + ".Mute"](bool value)->void {
				m_remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value));
			});
		if (m_type == Type::Voicemeeter) {
			break;
		}
	}
	timer.Set(::std::chrono::milliseconds{ 100 },
		[this]()->bool {
			long dirty{ m_remote.VBVMR_IsParametersDirty() };
			if (dirty < 0) {
				throw ::std::exception{ "Cannot check Voicemeeter" };
			}
			float value{ 0.F };
			if (Type::Voicemeeter < m_type && dirty) {
				m_remote.VBVMR_GetParameterFloat(const_cast<char*>("vban.Enable"), &value);
				m_mixer.set_Vban<Client>(0.01F < value);
			}
			for (auto& strip : m_mixer.get_PhysicalInput()) {
				if (dirty) {
					::std::string key{ ToBusKey(strip.get_Id()) };
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Gain").c_str()), &value);
					strip.set_Gain<Client>(value);
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Mute").c_str()), &value);
					strip.set_Mute<Client>(0.01F < value);
				}
				for (auto& line : strip) {
					long key{ ToChannelKey(line.get_Id()) };
					m_remote.VBVMR_GetLevel(static_cast<long>(LevelType::PostFaderInput), key, &value);
					line.set_Level<Client>(value);
				}
			}
			for (auto& strip : m_mixer.get_VirtualInput()) {
				if (dirty) {
					::std::string key{ ToBusKey(strip.get_Id()) };
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Gain").c_str()), &value);
					strip.set_Gain<Client>(value);
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Mute").c_str()), &value);
					strip.set_Mute<Client>(0.01F < value);
				}
				for (auto& line : strip) {
					long key{ ToChannelKey(line.get_Id()) };
					m_remote.VBVMR_GetLevel(static_cast<long>(LevelType::PostFaderInput), key, &value);
					line.set_Level<Client>(value);
				}
			}
			for (auto& strip : m_mixer.get_PhysicalOutput()) {
				if (dirty) {
					::std::string key{ ToBusKey(strip.get_Id()) };
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Gain").c_str()), &value);
					strip.set_Gain<Client>(value);
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Mute").c_str()), &value);
					strip.set_Mute<Client>(0.01F < value);
				}
				for (auto& line : strip) {
					long key{ ToChannelKey(line.get_Id()) };
					m_remote.VBVMR_GetLevel(static_cast<long>(LevelType::Output), key, &value);
					line.set_Level<Client>(value);
				}
				if (m_type == Type::Voicemeeter) {
					break;
				}
			}
			for (auto& strip : m_mixer.get_VirtualOutput()) {
				if (dirty) {
					::std::string key{ ToBusKey(strip.get_Id()) };
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Gain").c_str()), &value);
					strip.set_Gain<Client>(value);
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Mute").c_str()), &value);
					strip.set_Mute<Client>(0.01F < value);
				}
				for (auto& line : strip) {
					long key{ ToChannelKey(line.get_Id()) };
					m_remote.VBVMR_GetLevel(static_cast<long>(LevelType::Output), key, &value);
					line.set_Level<Client>(value);
				}
				if (m_type == Type::Voicemeeter) {
					break;
				}
			}
			return true;
		});
}