#include "Windows/Error.h"
#include "Windows/Messages.h"

#include "Mixer.h"

using namespace Voicemeeter::Remote;

Mixer::Mixer(
	::Environment::ITimer& envTimer
) : m_remote{}
  , m_network{ *this }
  , m_cpInput{}
  , m_cpOutput{}
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
	m_cpInput.push_back(::std::move(
		::std::make_unique<Input>(
			*this, ::std::string{ "Strip[0]" },
			*this, LevelType::PostFaderInput, 0L,
			*this, LevelType::PostFaderInput, 1L,
			::std::string{ "P" }
		)
	));
	m_cpOutput.emplace_back(::std::move(
		::std::make_unique<Output>(
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
		)
	));
	switch (m_type) {
	case Type::Voicemeeter: {
		m_cpInput.emplace_back(::std::move(
			::std::make_unique<Input>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
	} break;
	case Type::Banana: {
		m_cpInput.emplace_back(::std::move(
			::std::make_unique<Input>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
	} break;
	case Type::Potato: {
		m_cpInput.emplace_back(::std::move(
			::std::make_unique<Input>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
		m_cpOutput.emplace_back(::std::move(
			::std::make_unique<Output>(
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
			)
		));
	} break;
	}
	for (const ::std::unique_ptr<Input>& pInput : m_cpInput) {
		m_cInputPlugs[::std::ref(const_cast<Input&>(*pInput))];
	}
	for (const ::std::unique_ptr<Output>& pOutput : m_cpOutput) {
		m_cOutputPlugs[::std::ref(const_cast<Output&>(*pOutput))];
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
				&& !(::std::chrono::high_resolution_clock::now() - m_restart < ::std::chrono::milliseconds{ 2000 })
			};
			m_dirty = false;

			m_network.Update(isDirty);
			for (::std::unique_ptr<Input>& pInput : m_cpInput) {
				pInput->Update(isDirty);
			}
			for (::std::unique_ptr<Output>& pOutput : m_cpOutput) {
				pOutput->Update(isDirty);
			}

			if (!isDirty) {
				return true;
			}

			for (const ::std::unique_ptr<Input>& pInput : m_cpInput) {
				::std::unordered_set<::std::reference_wrapper<Output>>& inputPlugs{ m_cInputPlugs[::std::ref(*pInput)]};
				for (const ::std::unique_ptr<Output>& pOutput : m_cpOutput) {
					::std::unordered_set<::std::reference_wrapper<Input>>& outputPlugs{ m_cOutputPlugs[::std::ref(*pOutput)]};

					bool plug{ inputPlugs.find(::std::ref(*pOutput)) != inputPlugs.end() };
					float value{ get_Parameter(pInput->get_Key() + "." + pOutput->get_Label()) };
					if (!(::std::abs(plug - value) < 0.01)) {
						plug = !(value < 0.01);
						if (plug) {
							inputPlugs.insert(::std::ref(*pOutput));
							outputPlugs.insert(::std::ref(*pInput));
						} else {
							inputPlugs.erase(::std::ref(*pOutput));
							outputPlugs.erase(::std::ref(*pInput));
						}
						for (const ::std::function<void(bool)>& callback : m_cCallback[::std::make_pair(::std::ref(*pInput), ::std::ref(*pOutput))]) {
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
const ::std::vector<::std::unique_ptr<Input>>& Mixer::get_Inputs() const {
	return m_cpInput;
}
const ::std::vector<::std::unique_ptr<Output>>& Mixer::get_Outputs() const {
	return m_cpOutput;
}
const ::std::unordered_set<::std::reference_wrapper<Output>>& Mixer::get_Plugs(const Input& input) const {
	return m_cInputPlugs.find(::std::ref(const_cast<Input&>(input)))
		->second;
}
const ::std::unordered_set<::std::reference_wrapper<Input>>& Mixer::get_Plugs(const Output& output) const {
	return m_cOutputPlugs.find(::std::ref(const_cast<Output&>(output)))
		->second;
}
void Mixer::set_Plug(const Input& input, const Output& output, bool value) {
	set_Dirty();
	::std::reference_wrapper<Input> rInput{ const_cast<Input&>(input) };
	::std::reference_wrapper<Output> rOutput{ const_cast<Output&>(output) };
	if (value) {
		m_cInputPlugs.find(rInput)
			->second
				.insert(rOutput);
		m_cOutputPlugs.find(rOutput)
			->second
				.insert(rInput);
	} else {
		m_cInputPlugs.find(rInput)
			->second
				.erase(rOutput);
		m_cOutputPlugs.find(rOutput)
			->second
				.erase(rInput);
	}
	m_remote.VBVMR_SetParameterFloat(const_cast<char*>((input.get_Key() + "." + output.get_Label()).c_str()), value);
}
void Mixer::on_Plug(const Input& input, const Output& output, const ::std::function<void(bool)>& callback) {
	m_cCallback[::std::make_pair(::std::ref(const_cast<Input&>(input)), ::std::ref(const_cast<Output&>(output)))]
		.push_back(callback);
}