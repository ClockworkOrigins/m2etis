/*
 Copyright (2016) Michael Baer, Daniel Bonrath, All rights reserved.

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

#include "m2etis/pubsub/PubSubSystem.h"

#include "DCB.h"

const unsigned short NODE_AMOUNT = 4;
const uint16_t MESSAGE_AMOUNT = 50;
const unsigned int FIRSTCHANNEL = 0;
const unsigned int CHANNELAMOUNT = m2etis::pubsub::CHANNEL_COUNT;

void testChannel(m2etis::pubsub::ChannelName cn) {
	std::vector<std::shared_ptr<DCB>> _callbacks;
	std::vector<std::shared_ptr<m2etis::pubsub::PubSubSystem>> _pubSubs;

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs.push_back(std::shared_ptr<m2etis::pubsub::PubSubSystem>(new m2etis::pubsub::PubSubSystem("127.0.0.1", 12345 + i, "127.0.0.1", 12345, { "127.0.0.1" })));
		_callbacks.push_back(std::make_shared<DCB>());
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(375 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->subscribe<CharVectorEventType>(cn, *_callbacks[i]);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(375 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->publish(cn, CharVectorEventType());
	}

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_callbacks[i]->waitUntil(NODE_AMOUNT, 2000000);
		ASSERT_NEAR(NODE_AMOUNT, _callbacks[i]->_counter, NODE_AMOUNT / 2) << "Channel " << int(cn) << " of Node " << i << " has not correct amount of messages!";
	}

	for (unsigned short i = 0; i < MESSAGE_AMOUNT; ++i) {
		for (unsigned short j = 0; j < NODE_AMOUNT; ++j) {
			_pubSubs[j]->publish(cn, CharVectorEventType());
		}
	}

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_callbacks[i]->waitUntil(MESSAGE_AMOUNT * NODE_AMOUNT + NODE_AMOUNT, 10000000);
		ASSERT_NEAR(MESSAGE_AMOUNT * NODE_AMOUNT + NODE_AMOUNT, _callbacks[i]->_counter, (MESSAGE_AMOUNT * NODE_AMOUNT + NODE_AMOUNT) / 2) << "Channel " << int(cn) << " of Node " << i << " has not correct amount of messages!";
	}
}

TEST(M2etis, testChannelsForDelivery) {
	for (unsigned int i = FIRSTCHANNEL; i < CHANNELAMOUNT; i++) {
		std::cout << "Start Channel " << i << std::endl;
		testChannel(m2etis::pubsub::ChannelName(i));
		std::cout << "Finish Channel " << i << std::endl;
	}
}

void testUnsubscribe(m2etis::pubsub::ChannelName cn) {
	std::vector<std::shared_ptr<DCB>> _callbacks;
	std::vector<std::shared_ptr<m2etis::pubsub::PubSubSystem>> _pubSubs;

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs.push_back(std::shared_ptr<m2etis::pubsub::PubSubSystem>(new m2etis::pubsub::PubSubSystem("127.0.0.1", 12345 + i, "127.0.0.1", 12345, {"127.0.0.1"})));
		_callbacks.push_back(std::make_shared<DCB>());
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(375 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->subscribe<CharVectorEventType>(cn, *_callbacks[i]);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(375 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->publish(cn, CharVectorEventType());
	}

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_callbacks[i]->waitUntil(NODE_AMOUNT, 2000000);
		ASSERT_NEAR(NODE_AMOUNT, _callbacks[i]->_counter, NODE_AMOUNT / 2) << "Channel " << int(cn) << " of Node " << i << " has not correct amount of messages!";
	}

	for (unsigned short i = 0; i < MESSAGE_AMOUNT; ++i) {
		for (unsigned short j = 0; j < NODE_AMOUNT; ++j) {
			_pubSubs[j]->publish(cn, CharVectorEventType());
		}
	}

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_callbacks[i]->waitUntil(MESSAGE_AMOUNT * NODE_AMOUNT + NODE_AMOUNT, 10000000);
	}

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		_pubSubs[i]->unsubscribe<CharVectorEventType>(cn);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50 * NODE_AMOUNT));

	for (unsigned short i = 0; i < MESSAGE_AMOUNT; ++i) {
		for (unsigned short j = 0; j < NODE_AMOUNT; ++j) {
			_pubSubs[j]->publish(cn, CharVectorEventType());
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(200 * NODE_AMOUNT));

	for (unsigned short i = 0; i < NODE_AMOUNT; ++i) {
		ASSERT_NEAR(MESSAGE_AMOUNT * NODE_AMOUNT + NODE_AMOUNT, _callbacks[i]->_counter, (MESSAGE_AMOUNT * NODE_AMOUNT + NODE_AMOUNT) / 2) << "Channel " << int(cn) << " of Node " << i << " has not correct amount of messages!";
	}
}

TEST(M2etis, testChannelsForUnsubscribe) {
	for (unsigned int i = FIRSTCHANNEL; i < CHANNELAMOUNT; ++i) {
		std::cout << "Start Channel " << i << std::endl;
		testUnsubscribe(m2etis::pubsub::ChannelName(i));
		std::cout << "Finish Channel " << i << std::endl;
	}
}
