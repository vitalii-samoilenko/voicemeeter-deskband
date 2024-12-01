#include "Client.h"

using namespace ::Voicemeeter::Remote;

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
	timer.Set(::std::chrono::milliseconds{ 100 },
		[this]()->bool {
			long dirty{ m_remote.VBVMR_IsParametersDirty() };
			if (dirty < 0) {
				throw ::std::exception{ "Cannot check Voicemeeter" };
			}

			size_t size{ 1 };
			Update(m_mixer.get_PhysicalInput(), size, LevelType::PostFaderInput, dirty);
			Update(m_mixer.get_VirtualInput(), size, LevelType::PostFaderInput, dirty);
			if (m_type != Type::Voicemeeter) {
				size = 2;
			}
			Update(m_mixer.get_PhysicalOutput(), size, LevelType::Output, dirty);
			Update(m_mixer.get_VirtualOutput(), size, LevelType::Output, dirty);

			return true;
		});
}