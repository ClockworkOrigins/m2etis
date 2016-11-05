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

#ifndef __M2ETIS_INTERNALMESSAGE_TEST_H__
#define __M2ETIS_INTERNALMESSAGE_TEST_H__

#include "m2etis/message/MessageSerialization.h"
#include "m2etis/message/key/Key.h"
#include "m2etis/message/key/providers/IPv4KeyProvider.h"

#include "m2etis/pubsub/config/Strategies.h"

using namespace m2etis::pubsub::routing;
using namespace m2etis::pubsub::filter;
using namespace m2etis::pubsub::persistence;
using namespace m2etis::pubsub::validity;
using namespace m2etis::pubsub::security;
using namespace m2etis::pubsub::order;
using namespace m2etis::pubsub::partition;
using namespace m2etis::pubsub::deliver;
using namespace m2etis::pubsub::rendezvous;

typedef boost::shared_ptr<std::vector<unsigned char>> PP;
typedef m2etis::message::NetworkMessage<m2etis::net::NetworkType<m2etis::net::TCP>> NetMessage;
typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>,
	m2etis::pubsub::ChannelType<SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
	, NullFilter<std::vector<unsigned char>, m2etis::net::NetworkType<m2etis::net::TCP>>
	, NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
	, NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
	, NullPersistence
	, NullValidity
	, NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, std::vector<unsigned char>>
	, NullSecurity, NullRendezvous>, std::vector<unsigned char>> IMessage;

TEST(InternalMessageTest, Serialize) {
    IMessage::Ptr m1(new IMessage());
	std::string t = "test";
	PP payload(new std::vector<unsigned char>(t.begin(), t.end()));

	// construct Message
	m2etis::message::Key<m2etis::message::IPv4KeyProvider> key("127.0.0.1:6666");
	m1->sender = key;
	m2etis::message::Key<m2etis::message::IPv4KeyProvider> keyR("123.123.123:123");
	m1->receiver = keyR;
	m1->payload = payload;
    m1->type = m2etis::message::PUBLISH;
	EXPECT_EQ("127.0.0.1:6666", m1->sender.toStr());
	EXPECT_EQ("123.123.123.123:123", m1->receiver.toStr());

	// Serialize
	std::string y = m2etis::message::serialization::serializeNetworkMsg<NetMessage>(m1);
	// Deserialize
	NetMessage::Ptr mm2 = m2etis::message::serialization::deserializeNetworkMsg<m2etis::net::NetworkType<m2etis::net::TCP>>(y);
	IMessage m2(*static_cast<IMessage *>(mm2.get()));

	// Test for equality
    EXPECT_EQ(m2etis::message::PUBLISH, m2etis::message::ActionType(m2.type));

    IMessage::Ptr m3(new IMessage());
	std::string t3 = "test";
	PP payload3(new std::vector<unsigned char>(t.begin(), t.end()));

	// construct Message
	m2etis::message::Key<m2etis::message::IPv4KeyProvider> key3("127.0.0.1:6666");
	m3->sender = key3;
	m2etis::message::Key<m2etis::message::IPv4KeyProvider> keyR3("123.123.123:123");
	m3->receiver = keyR3;
	m3->payload = payload3;
    m3->type = m2etis::message::SUBSCRIBE;
	EXPECT_EQ("127.0.0.1:6666", m3->sender.toStr());
	EXPECT_EQ("123.123.123.123:123", m3->receiver.toStr());

	// Serialize
	std::string y3 = m2etis::message::serialization::serializeNetworkMsg<NetMessage>(m3);
	// Deserialize
	NetMessage::Ptr mm3 = m2etis::message::serialization::deserializeNetworkMsg<m2etis::net::NetworkType<m2etis::net::TCP>>(y3);
	IMessage m4(*static_cast<IMessage *>(mm3.get()));

	// Test for equality
    EXPECT_EQ("127.0.0.1:6666", m4.sender.toStr());
    EXPECT_EQ(m2etis::message::SUBSCRIBE, m2etis::message::ActionType(m4.type));
}

#endif /* __M2ETIS_INTERNALMESSAGE_TEST_H__ */
