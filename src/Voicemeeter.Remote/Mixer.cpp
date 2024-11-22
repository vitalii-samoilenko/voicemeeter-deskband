#include "Windows/Error.h"
#include "Windows/Messages.h"

#include "Mixer.h"

using namespace Voicemeeter::Remote;

Mixer::Mixer(
	::Environment::ITimer& envTimer
) : m_remote{}
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
	long login{ m_remote.VBVMR_Login() };
	if (login < 0) {
		throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot connect to Voicemeeter" };
	} else if (login) {
		m_remote.VBVMR_Logout();
		throw ::Windows::Error{ MSG_ERR_GENERAL, "Voicemeeter is not started" };
	}
	if (m_remote.VBVMR_GetVoicemeeterType(reinterpret_cast<long*>(&m_type))) {
		m_remote.VBVMR_Logout();
		throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot get Voicemeeter type" };
	}
	m_cInput.emplace(
		*this, ::std::string{ "Strip[0]" },
		*this, LevelType::PostFaderInput, 0L,
		*this, LevelType::PostFaderInput, 1L,
		::std::string{ "P" }
	);
	m_cOutput.emplace(
		*this, ::std::string{ "Bus[0]" },
		*this, LevelType::Output, 0L,
		*this, LevelType::Output, 1L,
		*this, LevelType::Output, 2L,
		*this, LevelType::Output, 3L,
		*this, LevelType::Output, 4L,
		*this, LevelType::Output, 5L,
		*this, LevelType::Output, 6L,
		*this, LevelType::Output, 7L,
		::std::string{ "A1" }
	);
	switch (m_type) {
	case Type::Voicemeeter: {
		m_cInput.emplace(
			*this, ::std::string{ "Strip[2]" },
			*this, LevelType::PostFaderInput, 4L,
			*this, LevelType::PostFaderInput, 5L,
			*this, LevelType::PostFaderInput, 6L,
			*this, LevelType::PostFaderInput, 7L,
			*this, LevelType::PostFaderInput, 8L,
			*this, LevelType::PostFaderInput, 9L,
			*this, LevelType::PostFaderInput, 10L,
			*this, LevelType::PostFaderInput, 11L,
			::std::string{ "V" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[1]" },
			*this, LevelType::Output, 8L,
			*this, LevelType::Output, 9L,
			*this, LevelType::Output, 10L,
			*this, LevelType::Output, 11L,
			*this, LevelType::Output, 12L,
			*this, LevelType::Output, 13L,
			*this, LevelType::Output, 14L,
			*this, LevelType::Output, 15L,
			::std::string{ "B1" }
		);
	} break;
	case Type::Banana: {
		m_cInput.emplace(
			*this, ::std::string{ "Strip[3]" },
			*this, LevelType::PostFaderInput, 6L,
			*this, LevelType::PostFaderInput, 7L,
			*this, LevelType::PostFaderInput, 8L,
			*this, LevelType::PostFaderInput, 9L,
			*this, LevelType::PostFaderInput, 10L,
			*this, LevelType::PostFaderInput, 11L,
			*this, LevelType::PostFaderInput, 12L,
			*this, LevelType::PostFaderInput, 13L,
			::std::string{ "V" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[1]" },
			*this, LevelType::Output, 8L,
			*this, LevelType::Output, 9L,
			*this, LevelType::Output, 10L,
			*this, LevelType::Output, 11L,
			*this, LevelType::Output, 12L,
			*this, LevelType::Output, 13L,
			*this, LevelType::Output, 14L,
			*this, LevelType::Output, 15L,
			::std::string{ "A2" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[3]" },
			*this, LevelType::Output, 24L,
			*this, LevelType::Output, 25L,
			*this, LevelType::Output, 26L,
			*this, LevelType::Output, 27L,
			*this, LevelType::Output, 28L,
			*this, LevelType::Output, 29L,
			*this, LevelType::Output, 30L,
			*this, LevelType::Output, 31L,
			::std::string{ "B1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[4]" },
			*this, LevelType::Output, 32L,
			*this, LevelType::Output, 33L,
			*this, LevelType::Output, 34L,
			*this, LevelType::Output, 35L,
			*this, LevelType::Output, 36L,
			*this, LevelType::Output, 37L,
			*this, LevelType::Output, 38L,
			*this, LevelType::Output, 39L,
			::std::string{ "B2" }
		);
	} break;
	case Type::Potato: {
		m_cInput.emplace(
			*this, ::std::string{ "Strip[5]" },
			*this, LevelType::PostFaderInput, 10L,
			*this, LevelType::PostFaderInput, 11L,
			*this, LevelType::PostFaderInput, 12L,
			*this, LevelType::PostFaderInput, 13L,
			*this, LevelType::PostFaderInput, 14L,
			*this, LevelType::PostFaderInput, 15L,
			*this, LevelType::PostFaderInput, 16L,
			*this, LevelType::PostFaderInput, 17L,
			::std::string{ "V" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[1]" },
			*this, LevelType::Output, 8L,
			*this, LevelType::Output, 9L,
			*this, LevelType::Output, 10L,
			*this, LevelType::Output, 11L,
			*this, LevelType::Output, 12L,
			*this, LevelType::Output, 13L,
			*this, LevelType::Output, 14L,
			*this, LevelType::Output, 15L,
			::std::string{ "A2" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[5]" },
			*this, LevelType::Output, 40L,
			*this, LevelType::Output, 41L,
			*this, LevelType::Output, 42L,
			*this, LevelType::Output, 43L,
			*this, LevelType::Output, 44L,
			*this, LevelType::Output, 45L,
			*this, LevelType::Output, 46L,
			*this, LevelType::Output, 47L,
			::std::string{ "B1" }
		);
		m_cOutput.emplace(
			*this, ::std::string{ "Bus[6]" },
			*this, LevelType::Output, 48L,
			*this, LevelType::Output, 49L,
			*this, LevelType::Output, 50L,
			*this, LevelType::Output, 51L,
			*this, LevelType::Output, 52L,
			*this, LevelType::Output, 53L,
			*this, LevelType::Output, 54L,
			*this, LevelType::Output, 55L,
			::std::string{ "B2" }
		);
	} break;
	}
	for (const Input& input : m_cInput) {
		m_cInputPlugs[
			reinterpret_cast<void*>(&
				const_cast<Input&>(input))];
	}
	for (const Output& output : m_cOutput) {
		m_cOutputPlugs[
			reinterpret_cast<void*>(&
				const_cast<Output&>(output))];
	}
	envTimer.Set(::std::chrono::milliseconds{ 100 },
		[this]()->bool {
			long dirty{ m_remote.VBVMR_IsParametersDirty() };
			if (dirty < 0) {
				throw ::Windows::Error{ MSG_ERR_GENERAL, "Cannot check Voicemeeter" };
			}
			bool isDirty{
				dirty
				&& !m_dirty
				&& !(::std::chrono::system_clock::now() - m_restart < ::std::chrono::milliseconds{ 2000 })
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
				Range<Output>& inputPlugs{ m_cInputPlugs[
					reinterpret_cast<void*>(&input)] };
				for (Output& output : m_cOutput) {
					Range<Input>& outputPlugs{ m_cOutputPlugs[
						reinterpret_cast<void*>(&output)] };
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
							reinterpret_cast<void*>(&input),
							reinterpret_cast<void*>(&output)
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

::Voicemeeter::Type Mixer::get_Type() const {
	return m_type;
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
	return m_cInputPlugs.find(
			reinterpret_cast<void*>(&
				const_cast<Input&>(input)))
		->second;
}
const Range<Input>& Mixer::get_Plugs(const Output& output) const {
	return m_cOutputPlugs.find(
			reinterpret_cast<void*>(&
				const_cast<Output&>(output)))
		->second;
}
void Mixer::set_Plug(const Input& input, const Output& output, bool value) {
	set_Dirty();
	void* pInput{
		reinterpret_cast<void*>(&
			const_cast<Input&>(input))
	};
	void* pOutput{
		reinterpret_cast<void*>(&
			const_cast<Output&>(output))
	};
	if (value) {
		m_cInputPlugs.find(pInput)
			->second
				.copy(output, m_cOutput);
		m_cOutputPlugs.find(pOutput)
			->second
				.copy(input, m_cInput);
	} else {
		m_cInputPlugs.find(pInput)
			->second
				.erase(output);
		m_cOutputPlugs.find(pOutput)
			->second
				.erase(input);
	}
	m_remote.VBVMR_SetParameterFloat(const_cast<char*>((input.get_Key() + "." + output.get_Label()).c_str()), value);
}
void Mixer::on_Plug(const Input& input, const Output& output, const ::std::function<void(bool)>& callback) {
	m_cCallback[::std::make_pair(
		reinterpret_cast<void*>(&
			const_cast<Input&>(input)),
		reinterpret_cast<void*>(&
			const_cast<Output&>(output))
	)].push_back(callback);
}