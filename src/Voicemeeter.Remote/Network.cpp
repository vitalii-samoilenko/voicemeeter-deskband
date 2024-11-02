#include <cmath>

#include "Mixer.h"

using namespace Voicemeeter::Remote;

Network::Network(
	Mixer& mixer
) : m_mixer{ mixer }
  , m_vban{}
  , m_cCallback{} {

}

bool Network::get_Supported() const {
	return Type::Voicemeeter < m_mixer.get_Type();
}
bool Network::get_Vban() const {
	return m_vban;
}
void Network::set_Vban(bool value) {
	m_mixer.set_Restart();
	m_vban = value;
	m_mixer.set_Parameter("vban.Enable", m_vban);
}
void Network::on_Vban(const ::std::function<void(bool)>& callback) {
	m_cCallback.push_back(callback);
}

void Network::Update(bool dirty) {
	if (!dirty) {
		return;
	}

	float value{ m_mixer.get_Parameter("vban.Enable") };
	if (!(::std::abs(m_vban - value) < 0.01)) {
		m_vban = !(value < 0.01);
		for (const ::std::function<void(bool)>& callback : m_cCallback) {
			callback(m_vban);
		}
	}
}