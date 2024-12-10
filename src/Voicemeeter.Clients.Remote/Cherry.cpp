#include <chrono>

#include "Cherry.h"

using namespace ::Voicemeeter::Clients::Remote;

static bool g_dirty{};

template<Type Remote>
inline static ::std::string ToBusKey(size_t id);
template<>
inline static ::std::string ToBusKey<Type::Voicemeeter>(size_t id) {
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
inline static ::std::string ToBusKey<Type::Banana>(size_t id) {
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
inline static ::std::string ToBusKey<Type::Potato>(size_t id) {
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
inline static ::std::string ToBusKey(Type type, size_t id) {
	switch (type)
	{
	case ::Voicemeeter::Clients::Remote::Type::Voicemeeter:
		return ToBusKey<::Voicemeeter::Clients::Remote::Type::Voicemeeter>(id);
		break;
	case ::Voicemeeter::Clients::Remote::Type::Banana:
		return ToBusKey<::Voicemeeter::Clients::Remote::Type::Banana>(id);
		break;
	case ::Voicemeeter::Clients::Remote::Type::Potato:
		return ToBusKey<::Voicemeeter::Clients::Remote::Type::Potato>(id);
		break;
	default:
		throw ::std::exception{ "Remote is not supported" };
		break;
	}
}

inline static ::std::string ToBusSubKey(size_t id) {
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

template<Type Remote>
inline static long ToChannelKey(size_t id);
template<>
inline static long ToChannelKey<Type::Voicemeeter>(size_t id) {
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
inline static long ToChannelKey<Type::Banana>(size_t id) {
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
inline static long ToChannelKey<Type::Potato>(size_t id) {
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
inline static long ToChannelKey(Type type, size_t id) {
	switch (type)
	{
	case ::Voicemeeter::Clients::Remote::Type::Voicemeeter:
		return ToChannelKey<::Voicemeeter::Clients::Remote::Type::Voicemeeter>(id);
		break;
	case ::Voicemeeter::Clients::Remote::Type::Banana:
		return ToChannelKey<::Voicemeeter::Clients::Remote::Type::Banana>(id);
		break;
	case ::Voicemeeter::Clients::Remote::Type::Potato:
		return ToChannelKey<::Voicemeeter::Clients::Remote::Type::Potato>(id);
		break;
	default:
		throw ::std::exception{ "Remote is not supported" };
		break;
	}
}

template<typename TStrip>
inline static void StripSubscribe(::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription, TStrip& strip, const T_VBVMR_INTERFACE& remote, Type type) {
	::std::string busKey{ ToBusKey(type, strip.get_Id()) };
	subscription.on_Gain(strip.get_Id(),
		[&remote, key = busKey + ".Gain"](double value)->void {
			if (remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value))) {
				throw ::std::exception{ ("Could not set " + key).c_str() };
			}
			g_dirty = true;
		});
	subscription.on_Mute(strip.get_Id(),
		[&remote, key = busKey + ".Mute"](bool value)->void {
			if (remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value))) {
				throw ::std::exception{ ("Could not set " + key).c_str() };
			}
			g_dirty = true;
		});
}
template<typename TInput, typename TOutput>
inline static void PlugSubscribe(::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription, TInput& input, TOutput& output, const T_VBVMR_INTERFACE& remote, Type type) {
	subscription.on_Plug(input.get_Id(), output.get_Id(),
		[&remote, key = ToBusKey(type, input.get_Id()) + "." + ToBusSubKey(output.get_Id())](bool value)->void {
			if (remote.VBVMR_SetParameterFloat(const_cast<char*>(key.c_str()), static_cast<float>(value))) {
				throw ::std::exception{ ("Could not set " + key).c_str() };
			}
			g_dirty = true;
		});
}

template<typename TStrip>
inline static void StripUpdate(TStrip& strip, const T_VBVMR_INTERFACE& remote, Type type) {
	::std::string key{ ToBusKey(type, strip.get_Id()) };
	float value{ 0.F };
	if (remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Gain").c_str()), &value)) {
		throw ::std::exception{ ("Could not get " + key + ".Gain").c_str() };
	}
	strip.set_Gain<Cherry>(value);
	if (remote.VBVMR_GetParameterFloat(const_cast<char*>((key + ".Mute").c_str()), &value)) {
		throw ::std::exception{ ("Could not get " + key + ".Mute").c_str() };
	}
	strip.set_Mute<Cherry>(0.01F < value);
}
template<typename TLine>
inline static void LineUpdate(TLine& line, LevelType levelType, const T_VBVMR_INTERFACE& remote, Type type) {
	long key{ ToChannelKey(type, line.get_Id()) };
	float value{ 0.F };
	if (remote.VBVMR_GetLevel(static_cast<long>(levelType), key, &value)) {
		throw ::std::exception{ ("Could not get " + ::std::to_string(key) + " level").c_str() };
	}
	line.set_Level<Cherry>(value);
}
template<typename TInput, typename TOutput>
inline static void PlugUpdate(TInput& input, TOutput& output, const T_VBVMR_INTERFACE& remote, Type type, ::Voicemeeter::Adapters::Multiclient::Cherry& mixer) {
	::std::string key{ ToBusKey(type, input.get_Id()) + "." + ToBusSubKey(output.get_Id()) };
	float value{ 0.F };
	if (remote.VBVMR_GetParameterFloat(const_cast<char*>(key.c_str()), &value)) {
		throw ::std::exception{ ("Could not get " + key).c_str() };
	}
	mixer.set_Plug<Cherry>(input, output, 0.01 < value);
}

void Cherry::Subscribe() const {
	::Voicemeeter::Adapters::Multiclient::CherrySubscription& subscription = m_mixer.get_Subscription<Cherry>();
	if (Type::Voicemeeter < m_type) {
		subscription.on_Vban(
			[&remote = m_remote](bool value)->void {
				if (remote.VBVMR_SetParameterFloat(const_cast<char*>("vban.Enable"), static_cast<float>(value))) {
					throw ::std::exception{ "Could not set VBAN" };
				}
				g_dirty = true;
			});
	}
	for (auto& input : m_mixer.get_PhysicalInput()) {
		StripSubscribe(subscription, input, m_remote, m_type);
		for (auto& output : m_mixer.get_PhysicalOutput()) {
			PlugSubscribe(subscription, input, output, m_remote, m_type);
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
		for (auto& output : m_mixer.get_VirtualOutput()) {
			PlugSubscribe(subscription, input, output, m_remote, m_type);
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
	}
	for (auto& input : m_mixer.get_VirtualInput()) {
		StripSubscribe(subscription, input, m_remote, m_type);
		for (auto& output : m_mixer.get_PhysicalOutput()) {
			PlugSubscribe(subscription, input, output, m_remote, m_type);
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
		for (auto& output : m_mixer.get_VirtualOutput()) {
			PlugSubscribe(subscription, input, output, m_remote, m_type);
			if (m_type == Type::Voicemeeter) {
				break;
			}
		}
	}
	for (auto& output : m_mixer.get_PhysicalOutput()) {
		StripSubscribe(subscription, output, m_remote, m_type);
		if (m_type == Type::Voicemeeter) {
			break;
		}
	}
	for (auto& output : m_mixer.get_VirtualOutput()) {
		StripSubscribe(subscription, output, m_remote, m_type);
		if (m_type == Type::Voicemeeter) {
			break;
		}
	}
}
void Cherry::Update() const {
	long remoteDirty{ m_remote.VBVMR_IsParametersDirty() };
	if (remoteDirty < 0) {
		throw ::std::exception{ "Cannot check Voicemeeter" };
	}
	bool dirty{
		remoteDirty
		&& !g_dirty
	};
	g_dirty = false;
	float value{ 0.F };
	if (Type::Voicemeeter < m_type && dirty) {
		if (m_remote.VBVMR_GetParameterFloat(const_cast<char*>("vban.Enable"), &value)) {
			throw ::std::exception{ "Could not get VBAN" };
		}
		m_mixer.set_Vban<Cherry>(0.01F < value);
	}
	for (auto& input : m_mixer.get_PhysicalInput()) {
		if (dirty) {
			StripUpdate(input, m_remote, m_type);
			for (auto& output : m_mixer.get_PhysicalOutput()) {
				PlugUpdate(input, output, m_remote, m_type, m_mixer);
				if (m_type == Type::Voicemeeter) {
					break;
				}
			}
			for (auto& output : m_mixer.get_VirtualOutput()) {
				PlugUpdate(input, output, m_remote, m_type, m_mixer);
				if (m_type == Type::Voicemeeter) {
					break;
				}
			}
		}
		for (auto& line : input) {
			LineUpdate(line, LevelType::PostFaderInput, m_remote, m_type);
		}
	}
	for (auto& input : m_mixer.get_VirtualInput()) {
		if (dirty) {
			StripUpdate(input, m_remote, m_type);
			for (auto& output : m_mixer.get_PhysicalOutput()) {
				PlugUpdate(input, output, m_remote, m_type, m_mixer);
				if (m_type == Type::Voicemeeter) {
					break;
				}
			}
			for (auto& output : m_mixer.get_VirtualOutput()) {
				PlugUpdate(input, output, m_remote, m_type, m_mixer);
				if (m_type == Type::Voicemeeter) {
					break;
				}
			}
		}
		for (auto& line : input) {
			LineUpdate(line, LevelType::PostFaderInput, m_remote, m_type);
		}
	}
	for (auto& output : m_mixer.get_PhysicalOutput()) {
		if (dirty) {
			StripUpdate(output, m_remote, m_type);
		}
		for (auto& line : output) {
			LineUpdate(line, LevelType::Output, m_remote, m_type);
		}
		if (m_type == Type::Voicemeeter) {
			break;
		}
	}
	for (auto& output : m_mixer.get_VirtualOutput()) {
		if (dirty) {
			StripUpdate(output, m_remote, m_type);
		}
		for (auto& line : output) {
			LineUpdate(line, LevelType::Output, m_remote, m_type);
		}
		if (m_type == Type::Voicemeeter) {
			break;
		}
	}
}