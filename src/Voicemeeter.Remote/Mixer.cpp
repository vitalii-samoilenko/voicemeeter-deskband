#include <cmath>

#include "Mixer.h"

using namespace Voicemeeter::Remote;

Mixer::Mixer(
	::Environment::ITimer& timer
) : m_timer{ timer }
  , m_remote{}
  , m_network{ *this }
  , m_cInput{}
  , m_cOutput{}
  , m_cInputPlugs{}
  , m_cOutputPlugs{}
  , m_cCallback{}
  , m_dirty{}
  , m_restart{}
  , m_type{} {
	if (InitializeDLLInterfaces(m_remote) != 0) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot initialize interfaces" };
	}
	if (m_remote.VBVMR_Login()) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot connect to Voicemeeter" };
	}
	if (m_remote.VBVMR_GetVoicemeeterType(&m_type)) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot get Voicemeeter type" };
	}
	m_cInput.emplace(
		*this, ::std::string{ "Strip[0]" },
		*this, 1L, 0L,
		*this, 1L, 1L,
		::std::string{ "P" }
	);
	switch (m_type) {
	case 1: {
		m_cInput.emplace(
			*this, ::std::string{ "Strip[2]" },
			*this, 1L, 4L,
			*this, 1L, 5L,
			::std::string{ "V" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[0]" },
			*this, 3L, 0L,
			*this, 3L, 1L,
			::std::string{ "A1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[1]" },
			*this, 3L, 8L,
			*this, 3L, 9L,
			::std::string{ "B1" }
		);
	} break;
	case 2: {
		m_cInput.emplace(
			*this, ::std::string{ "Strip[3]" },
			*this, 1L, 6L,
			*this, 1L, 7L,
			::std::string{ "V" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[0]" },
			*this, 3L, 0L,
			*this, 3L, 1L,
			::std::string{ "A1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[1]" },
			*this, 3L, 8L,
			*this, 3L, 9L,
			::std::string{ "A2" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[3]" },
			*this, 3L, 24L,
			*this, 3L, 25L,
			::std::string{ "B1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[4]" },
			*this, 3L, 32L,
			*this, 3L, 33L,
			::std::string{ "B2" }
		);
	} break;
	case 3: {
		m_cInput.emplace(
			*this, ::std::string{ "Strip[5]" },
			*this, 1L, 10L,
			*this, 1L, 11L,
			::std::string{ "V" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[0]" },
			*this, 3L, 0L,
			*this, 3L, 1L,
			::std::string{ "A1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[1]" },
			*this, 3L, 8L,
			*this, 3L, 9L,
			::std::string{ "A2" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[5]" },
			*this, 3L, 40L,
			*this, 3L, 41L,
			::std::string{ "B1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[6]" },
			*this, 3L, 48L,
			*this, 3L, 49L,
			::std::string{ "B2" }
		);
	} break;
	}
	for (const Input& input : m_cInput) {
		m_cInputPlugs[reinterpret_cast<unsigned long long>(&input)];
	}
	for (const Output& output : m_cOutput) {
		m_cOutputPlugs[reinterpret_cast<unsigned long long>(&output)];
	}
	m_timer.Set(::std::chrono::milliseconds{ 100 },
		[this]()->bool {
			long dirty{ m_remote.VBVMR_IsParametersDirty() };
			if (dirty < 0) {
				throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot check Voicemeeter" };
			}
			bool isDirty{
				dirty
				&& !m_dirty
				&& !(::std::chrono::system_clock::now() - m_restart < ::std::chrono::milliseconds{2000})
			};
			m_dirty = false;

			m_network.Update(isDirty);
			for (Input& input : m_cInput) {
				input.Update(isDirty);
			}
			for (Output& output : m_cOutput) {
				output.Update(isDirty);
			}

			if (!isDirty) {
				return true;
			}

			for (Input& input : m_cInput) {
				Range<Output>& inputPlugs{ m_cInputPlugs[reinterpret_cast<unsigned long long>(&input)] };
				for (Output& output : m_cOutput) {
					Range<Input>& outputPlugs{ m_cOutputPlugs[reinterpret_cast<unsigned long long>(&output)] };
					bool plug{ inputPlugs.find(output) != inputPlugs.end() };

					float value{ get_Parameter(input.get_Key() + "." + output.get_Label()) };
					if (!(::std::abs(plug - value) < 0.01)) {
						plug = !(value < 0.01);
						if (plug) {
							inputPlugs.copy(output, m_cOutput);
							outputPlugs.copy(input, m_cInput);
						} else {
							inputPlugs.erase(output);
							outputPlugs.erase(input);
						}
						for (const ::std::function<void(bool)>& callback : m_cCallback[::std::make_pair(
							reinterpret_cast<unsigned long long>(&input),
							reinterpret_cast<unsigned long long>(&output)
						)]) {
							callback(plug);
						}
					}
				}
			}

			return true;
		});
}

Mixer::~Mixer() {
	m_remote.VBVMR_Logout();
}

float Mixer::get_Parameter(const ::std::string& name) const {
	float value{};
	if (m_remote.VBVMR_GetParameterFloat(const_cast<char*>(name.c_str()), &value)) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, ("Cannot get Voicemeeter parameter: " + name).c_str() };
	}
	return value;
}
float Mixer::get_Level(long type, long channel) const {
	float value{};
	if (m_remote.VBVMR_GetLevel(type, channel, &value)) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, ("Cannot get Voicemeeter level: " + ::std::to_string(type) + " " + ::std::to_string(channel)).c_str()};
	}
	return value;
}
void Mixer::set_Parameter(const ::std::string& name, float value) {
	if (m_remote.VBVMR_SetParameterFloat(const_cast<char*>(name.c_str()), value)) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, ("Cannot set Voicemeeter parameter: " + name).c_str() };
	}
}

Network& Mixer::get_Network() {
	return m_network;
}
const Range<Input>& Mixer::get_Inputs() const {
	return m_cInput;
}
const Range<Output>& Mixer::get_Outputs() const {
	return m_cOutput;
}
const Range<Output>& Mixer::get_Plugs(const Input& input) const {
	return m_cInputPlugs.find(reinterpret_cast<unsigned long long>(&input))
		->second;
}
const Range<Input>& Mixer::get_Plugs(const Output& output) const {
	return m_cOutputPlugs.find(reinterpret_cast<unsigned long long>(&output))
		->second;
}
void Mixer::set_Plug(const Input& input, const Output& output, bool value) {
	set_Dirty();
	if (value) {
		m_cInputPlugs.find(reinterpret_cast<unsigned long long>(&input))
			->second
				.copy(output, m_cOutput);
		m_cOutputPlugs.find(reinterpret_cast<unsigned long long>(&output))
			->second
				.copy(input, m_cInput);
	} else {
		m_cInputPlugs.find(reinterpret_cast<unsigned long long>(&input))
			->second
				.erase(output);
		m_cOutputPlugs.find(reinterpret_cast<unsigned long long>(&output))
			->second
				.erase(input);
	}
	m_remote.VBVMR_SetParameterFloat(const_cast<char*>((input.get_Key() + "." + output.get_Label()).c_str()), value);
}
void Mixer::on_Plug(const Input& input, const Output& output, const ::std::function<void(bool)>& callback) {
	m_cCallback[::std::make_pair(
		reinterpret_cast<unsigned long long>(&input),
		reinterpret_cast<unsigned long long>(&output)
	)].push_back(callback);
}