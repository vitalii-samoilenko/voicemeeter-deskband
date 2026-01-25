#include <chrono>

#include "Cherry.h"

using namespace ::Voicemeeter::Clients::Remote;


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

template<typename TLine>
inline static void LineUpdate(TLine& line, LevelType levelType, const T_VBVMR_INTERFACE& remote, Type type) {
	long key{ ToChannelKey(type, line.get_Id()) };
	float value{ 0.F };
	if (remote.VBVMR_GetLevel(static_cast<long>(levelType), key, &value)) {
		throw ::std::exception{ ("Could not get " + ::std::to_string(key) + " level").c_str() };
	}
	line.set_Level<Cherry>(value);
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
