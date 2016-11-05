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

#ifndef __M2ETIS_NETWORKMESSAGE_TEST_H__
#define __M2ETIS_NETWORKMESSAGE_TEST_H__

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

#ifdef M2ETIS_BUILD_SHARED
	typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::TCP>, m2etis::pubsub::ChannelType<
		m2etis::pubsub::routing::SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
		, m2etis::pubsub::filter::NullFilter<CharVectorEventType, m2etis::net::NetworkType<m2etis::net::TCP>>
		, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
		, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
		, m2etis::pubsub::persistence::NullPersistence
		, m2etis::pubsub::validity::NullValidity
		, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, CharVectorEventType>
		, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, CharVectorEventType> Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage;
	BOOST_CLASS_EXPORT_GUID(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, "28")
	BOOST_CLASS_IMPLEMENTATION(Spreadit_Null_Null_Null_Null_Null_Null_Null_CharVector_TCPMessage, boost::serialization::object_serializable)
#endif

namespace NetworkMesageTest {

	typedef boost::shared_ptr<std::vector<unsigned char>> PP;
	typedef m2etis::message::NetworkMessage<m2etis::net::NetworkType<m2etis::net::TCP>> NetMessage;
	typedef m2etis::message::InternalMessage < m2etis::net::NetworkType<m2etis::net::TCP>,
		m2etis::pubsub::ChannelType<SpreaditRouting<m2etis::net::NetworkType<m2etis::net::TCP>>
		, NullFilter<std::vector<unsigned char>, m2etis::net::NetworkType<m2etis::net::TCP>>
		, NullOrder<m2etis::net::NetworkType<m2etis::net::TCP>>
		, NullDeliver<m2etis::net::NetworkType<m2etis::net::TCP>>
		, NullPersistence
		, NullValidity
		, NullPartition<m2etis::net::NetworkType<m2etis::net::TCP>, std::vector<unsigned char>>
		, NullSecurity, NullRendezvous>, std::vector<unsigned char>> IMessage;

	TEST(NetworkMessageTest, Serialize) {
		NetMessage::Ptr m1(new IMessage());
		std::string t = "test";
		PP payload(new std::vector<unsigned char>(t.begin(), t.end()));

		// construct Message
		m2etis::message::Key<m2etis::message::IPv4KeyProvider> key("127.0.0.1:6666");
		m1->sender = key;
		m2etis::message::Key<m2etis::message::IPv4KeyProvider> keyR("123.123.123:123");
		m1->receiver = keyR;
		EXPECT_EQ("123.123.123.123:123", m1->receiver.toStr());

		// Serialize
		std::string y = m2etis::message::serialization::serializeNetworkMsg<NetMessage>(m1);
		// Deserialize
		NetMessage::Ptr mm2 = m2etis::message::serialization::deserializeNetworkMsg<m2etis::net::NetworkType<m2etis::net::TCP>>(y);
		IMessage::Ptr m2(boost::static_pointer_cast<IMessage>(mm2));

		NetMessage::Ptr m3(new IMessage());
		std::string t3 = "test";
		PP payload3(new std::vector<unsigned char>(t.begin(), t.end()));

		// construct Message
		m2etis::message::Key<m2etis::message::IPv4KeyProvider> key3("127.0.0.1:6666");
		m3->sender = key3;
		m2etis::message::Key<m2etis::message::IPv4KeyProvider> keyR3("123.123.123:123");
		m3->receiver = keyR3;
		EXPECT_EQ("127.0.0.1:6666", m3->sender.toStr());
		EXPECT_EQ("123.123.123.123:123", m3->receiver.toStr());

		// Serialize
		std::string y3 = m2etis::message::serialization::serializeNetworkMsg<NetMessage>(m3);
		// Deserialize
		NetMessage::Ptr mm3 = m2etis::message::serialization::deserializeNetworkMsg<m2etis::net::NetworkType<m2etis::net::TCP>>(y3);
		IMessage::Ptr m4(boost::static_pointer_cast<IMessage>(mm3));

		// Test for equality
		EXPECT_EQ("127.0.0.1:6666", m4->sender.toStr());
	}

} /* namespace NetworkMessageTest */

#endif /* __M2ETIS_NETWORKMESSAGE_TEST_H__ */
