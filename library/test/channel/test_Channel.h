/**
 Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "m2etis/message/MessageSerialization.h"
#include "m2etis/pubsub/PubSubSystem.h"

#include "DCB.h"

#include "boost/thread.hpp"

const unsigned short NODE_AMOUNT = 2;
const unsigned int FIRSTCHANNEL = 0;
const unsigned int CHANNELAMOUNT = m2etis::pubsub::CHANNEL_COUNT;

void testSerialization(m2etis::pubsub::ChannelName cn) {
	m2etis::pubsub::PubSubSystem ps("127.0.0.1", 12345, "127.0.0.1", 12345, { "127.0.0.1" });
	std::string s;

	m2etis::message::M2etisMessage::Ptr mm = ps.createMessage<CharVectorEventType>(cn);

	*mm->typePtr = m2etis::message::SUBSCRIBE;

	s = m2etis::message::serialization::serializeNetworkMsg<m2etis::message::M2etisMessage>(mm);

	EXPECT_LT(s.size(), 80u) << "Subscribe of Channel " << int(cn) << " with size of " << s.size() << " to big!";

	*mm->typePtr = m2etis::message::UNSUBSCRIBE;

	s = m2etis::message::serialization::serializeNetworkMsg<m2etis::message::M2etisMessage>(mm);

	EXPECT_LT(s.size(), 80u) << "Unsubscribe of Channel " << int(cn) << " with size of " << s.size() << " to big!";

	*mm->typePtr = m2etis::message::CONTROL;

	s = m2etis::message::serialization::serializeNetworkMsg<m2etis::message::M2etisMessage>(mm);

	EXPECT_LT(s.size(), 80u) << "Control of Channel " << int(cn) << " with size of " << s.size() << " to big!";

	*mm->typePtr = m2etis::message::PUBLISH;

	s = m2etis::message::serialization::serializeNetworkMsg<m2etis::message::M2etisMessage>(mm);

	EXPECT_LT(s.size(), 50u) << "Publish of Channel " << int(cn) << " with size of " << s.size() << " to big!";

	*mm->typePtr = m2etis::message::NOTIFY;

	s = m2etis::message::serialization::serializeNetworkMsg<m2etis::message::M2etisMessage>(mm);

	EXPECT_LT(s.size() , 50u) << "Notify of Channel " << int(cn) << " with size of " << s.size() << " to big!";
}

TEST(M2etis, testSerializedMessages) {
	for (unsigned int i = FIRSTCHANNEL; i < CHANNELAMOUNT - 1; ++i) {
		std::cout << "Start Channel " << i << std::endl;
		testSerialization(m2etis::pubsub::ChannelName(i));
		std::cout << "Finish Channel " << i << std::endl;
	}
}

void testChannel(m2etis::pubsub::ChannelName cn) {
	std::vector<boost::shared_ptr<DCB>> _callbacks;
	std::vector<boost::shared_ptr<m2etis::pubsub::PubSubSystem>> _pubSubs;

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs.push_back(boost::shared_ptr<m2etis::pubsub::PubSubSystem>(new m2etis::pubsub::PubSubSystem("127.0.0.1", 12345 + i, "127.0.0.1", 12345, { "127.0.0.1" })));
		_callbacks.push_back(boost::make_shared<DCB>());
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->subscribe<CharVectorEventType>(cn, *_callbacks[i]);
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		m2etis::message::M2etisMessage::Ptr m = _pubSubs[i]->createMessage<CharVectorEventType>(cn);
		_pubSubs[i]->publish<CharVectorEventType>(cn, m);
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		ASSERT_EQ(NODE_AMOUNT, _callbacks[i]->_counter) << "Channel " << cn << " lost messages!";
	}

	for (unsigned short i = 0; i < 100; ++i) {
		for (unsigned short j = 0; j < NODE_AMOUNT; ++j) {
			m2etis::message::M2etisMessage::Ptr m = _pubSubs[j]->createMessage<CharVectorEventType>(cn);
			_pubSubs[j]->publish<CharVectorEventType>(cn, m);
		}
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		ASSERT_EQ(100 * NODE_AMOUNT + NODE_AMOUNT, _callbacks[i]->_counter) << "Channel " << cn << " lost messages!";
	}
}

TEST(M2etis, testChannelsForDelivery) {
	for (unsigned int i = FIRSTCHANNEL; i < CHANNELAMOUNT - 1; i++) {
		std::cout << "Start Channel " << i << std::endl;
		testChannel(m2etis::pubsub::ChannelName(i));
		std::cout << "Finish Channel " << i << std::endl;

		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
}

void testUnsubscribe(m2etis::pubsub::ChannelName cn) {
	std::vector<boost::shared_ptr<DCB>> _callbacks;
	std::vector<boost::shared_ptr<m2etis::pubsub::PubSubSystem>> _pubSubs;

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs.push_back(boost::shared_ptr<m2etis::pubsub::PubSubSystem>(new m2etis::pubsub::PubSubSystem("127.0.0.1", 12345 + i, "127.0.0.1", 12345, {"127.0.0.1"})));
		_callbacks.push_back(boost::make_shared<DCB>());
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->subscribe<CharVectorEventType>(cn, *_callbacks[i]);
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		m2etis::message::M2etisMessage::Ptr m = _pubSubs[i]->createMessage<CharVectorEventType>(cn);
		_pubSubs[i]->publish<CharVectorEventType>(cn, m);
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		ASSERT_EQ(NODE_AMOUNT, _callbacks[i]->_counter) << "Channel " << cn << " lost messages!";
	}

	for (unsigned short i = 0; i < 50; ++i) {
		for (unsigned short j = 0; j < NODE_AMOUNT; ++j) {
			m2etis::message::M2etisMessage::Ptr m = _pubSubs[j]->createMessage<CharVectorEventType>(cn);
			_pubSubs[j]->publish<CharVectorEventType>(cn, m);
		}
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->unsubscribe<CharVectorEventType>(cn);
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < 50; ++i) {
		for (unsigned short j = 0; j < NODE_AMOUNT; ++j) {
			m2etis::message::M2etisMessage::Ptr m = _pubSubs[j]->createMessage<CharVectorEventType>(cn);
			_pubSubs[j]->publish<CharVectorEventType>(cn, m);
		}
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(2000 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		ASSERT_EQ(50 * NODE_AMOUNT + NODE_AMOUNT, _callbacks[i]->_counter) << "Channel " << cn << " lost messages!";
	}
}

TEST(M2etis, testChannelsForUnsubscribe) {
	for (unsigned int i = FIRSTCHANNEL; i < CHANNELAMOUNT - 1; ++i) {
		std::cout << "Start Channel " << i << std::endl;
		testUnsubscribe(m2etis::pubsub::ChannelName(i));
		std::cout << "Finish Channel " << i << std::endl;

		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}
}
