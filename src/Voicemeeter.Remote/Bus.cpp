#include "Bus.h"

using namespace Voicemeeter::Remote;

Bus::Bus(
	const ::std::string& label
) : m_label{ label }
  , m_cpChannel{} {

}

const ::std::string& Bus::get_Label() const {
	return m_label;
}
const ::std::vector<::std::unique_ptr<Channel>>& Bus::get_Channels() const {
	return m_cpChannel;
}

void Bus::Update(bool dirty) {
	for (const ::std::unique_ptr<Channel>& pChannel : m_cpChannel) {
		pChannel->Update();
	}
}