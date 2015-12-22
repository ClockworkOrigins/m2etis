#include <iostream>

#include "m2etis/pubsub/DeliverCallbackInterface.h"
#include "m2etis/pubsub/PubSubSystem.h"
#include "m2etis/pubsub/config/ChannelName.h"

class DCB : public m2etis::pubsub::DeliverCallbackInterface {
public:
	void deliverCallback(const m2etis::message::M2etisMessage::Ptr m) {
		std::cout << this << " received a message!" << std::endl;
	}
};

int main() {
	DCB d1, d2;
	m2etis::pubsub::PubSubSystem p1("127.0.0.1", 12345, "127.0.0.1", 12345, { "127.0.0.1" });
	m2etis::pubsub::PubSubSystem p2("127.0.0.1", 12346, "127.0.0.1", 12345, { "127.0.0.1" });

	p1.subscribe<CharVectorEventType>(m2etis::pubsub::ChannelName::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, d1);
	p2.subscribe<CharVectorEventType>(m2etis::pubsub::ChannelName::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, d2);

	m2etis::message::M2etisMessage::Ptr m1 = p1.createMessage<CharVectorEventType>(m2etis::pubsub::ChannelName::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP);
	p1.publish<CharVectorEventType>(m2etis::pubsub::ChannelName::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, m1);

	m2etis::message::M2etisMessage::Ptr m2 = p1.createMessage<CharVectorEventType>(m2etis::pubsub::ChannelName::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP);
	p2.publish<CharVectorEventType>(m2etis::pubsub::ChannelName::TEST_Direct_Null_Null_Null_Null_Null_Null_Null_CharVector_TCP, m2);

	system("PAUSE");

	return 0;
}