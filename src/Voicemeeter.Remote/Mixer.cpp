#include <cmath>

#include "Windows/Error.h"

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
  , m_restart{} {
	if (InitializeDLLInterfaces(m_remote) != 0) {
		throw ::Windows::Error{ "Cannot initialize interfaces" };
	}
	if (m_remote.VBVMR_Login()) {
		throw ::Windows::Error{ "Cannot connect to Voicemeeter" };
	}
	m_cInput.emplace(
		*this, ::std::string{ "Strip[0]" },
		*this, 1L, 0L,
		*this, 1L, 1L,
		::std::string{ "P" }
	);
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
	for (const Input& input : m_cInput) {
		m_cInputPlugs[reinterpret_cast<unsigned long long>(&input)];
	}
	for (const Output& output : m_cOutput) {
		m_cOutputPlugs[reinterpret_cast<unsigned long long>(&output)];
	}
	m_timer.Set(::std::chrono::milliseconds{ 100 },
		[this]()->bool {
			bool isDirty{
				m_remote.VBVMR_IsParametersDirty()
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

					float value{};
					m_remote.VBVMR_GetParameterFloat(const_cast<char*>((input.get_Key() + "." + output.get_Label()).c_str()), &value);
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