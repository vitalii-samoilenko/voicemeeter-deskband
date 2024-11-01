#include <algorithm>

#include "Mixer.h"

using namespace Voicemeeter::Remote;

Channel::Channel(
	Mixer& mixer,
	long type,
	long channel
) : m_mixer{ mixer }
  , m_type{ type }
  , m_channel{ channel }
  , m_level{ 0 }
  , m_cCallback{} {

}

double Channel::get_Level() const {
	return m_level;
}

void Channel::on_Level(const ::std::function<void(double)>& callback) {
	m_cCallback.push_back(callback);
}

void Channel::Update() {
	float level{ 0 };
	m_mixer.get_Remote()
		.VBVMR_GetLevel(m_type, m_channel, &level);

	if (::std::abs(m_level - level) < 0.01) {
		return;
	}
	m_level = level;
	for (const ::std::function<void(double)>& callback : m_cCallback) {
		callback(m_level);
	}
}