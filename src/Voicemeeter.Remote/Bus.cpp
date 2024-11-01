#include "Bus.h"

using namespace Voicemeeter::Remote;

Bus::Bus(
	const ::std::string& label
) : m_label{ label }
  , m_cChannel{ } {

}

const ::std::string& Bus::get_Label() const {
	return m_label;
}
const Range<Channel>& Bus::get_Channels() const {
	return m_cChannel;
}

void Bus::Update(bool dirty) {
	for (Channel& channel : m_cChannel) {
		channel.Update();
	}
}