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

#include "m2etis/pubsub/Channel.h"
#include "m2etis/pubsub/config/ChannelConfiguration.h"

#include "m2etis/sim/OmNetMediator.h"
#include "m2etis/sim/PubSubWrapper.h"

namespace m2etis {
namespace sim {

	void OmNetMediator::registerOmNetCallback(OmNetCallbackInterface * o) {
		_ocallback = o;
	}

	void OmNetMediator::setCallback(net::NetworkCallbackInterface<net::NetworkType<net::OMNET>> * cb) {
		_callback = cb;
	}

	// send used by pubsub system
	void OmNetMediator::send(const message::ActionType type, const std::string & to, const message::M2Message<SimulationEventType>::Ptr msg, net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const hint, unsigned int size) {
		_ocallback->send(type, to, msg, hint, size);
	}

	void OmNetMediator::send(const net::NetworkType<net::OMNET>::Message::Ptr msg, net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const hint) {
		std::string s = message::serialization::serializeNetworkMsg<message::NetworkMessage<net::NetworkType<net::OMNET>>>(msg);
		message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr msg2 = message::NetworkMessage<net::NetworkType<net::OMNET> >::Ptr(message::serialization::deserializeNetworkMsg<net::NetworkType<net::OMNET>>(s));
		msg2->pl = msg->pl;
		msg2->sender = msg->sender;
		message::M2Message<SimulationEventType>::Ptr m1 = _pubsub->transformToM2etisMessage(msg, static_cast<pubsub::ChannelName>(msg->pl._simChannel));
		message::M2Message<SimulationEventType>::Ptr m = _pubsub->transformToM2etisMessage(msg2, static_cast<pubsub::ChannelName>(msg->pl._simChannel));

		m->payload = m1->payload;

		send(static_cast<message::ActionType>(*msg2->typePtr), msg->receiver.toStr(), m, hint, s.size());
	}

	void OmNetMediator::send(const net::NetworkType<net::OMNET>::Message::Ptr msg) {
		std::string s = message::serialization::serializeNetworkMsg<message::NetworkMessage<net::NetworkType<net::OMNET>>>(msg);
		message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr msg2 = message::NetworkMessage<net::NetworkType<net::OMNET> >::Ptr(message::serialization::deserializeNetworkMsg<net::NetworkType<net::OMNET>>(s));
		msg2->pl = msg->pl;
		message::M2Message<SimulationEventType>::Ptr m = _pubsub->transformToM2etisMessage(msg2, static_cast<pubsub::ChannelName>(msg->pl._simChannel));
		send(static_cast<message::ActionType>(*msg2->typePtr), msg2->receiver.toStr(), m, getSelfNodeHandle(), s.size());
	}

	net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr OmNetMediator::getSelfNodeHandle() const {
		net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr node(new net::NodeHandle<net::NetworkType<net::OMNET>>);
		std::string key;
		std::stringstream os;

		os << _name << ":" << _listenPort;
		key = os.str();

		net::NetworkType<net::OMNET>::Key tmp(key);

		node->key_ = tmp;
		node->hostname_ = _name;
		node->port_ = _listenPort;

		return node;
	}

	void OmNetMediator::registerMessageType(const message::ActionType type, const bool ack) const {
		// not needed by Simulator, yet
		assert(false);
	}

	void OmNetMediator::registerMessageType(const net::NetworkType<net::OMNET>::MessageType type, const bool ack) const {
		// not needed by Simulator, yet
		// M2ETIS_LOG_INFO("OmNetMediator::registerMessageType", "called with type " + boost::lexical_cast<std::string>(type) + " and ack " + boost::lexical_cast<std::string>(ack));
	}

	// Methods used by underlying Network to send Messages to m2etis
	bool OmNetMediator::forward(std::string & key, message::M2Message<SimulationEventType>::Ptr msg, const net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr hint) {
		message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr nm = _pubsub->transformToNetworkMessage(msg, static_cast<pubsub::ChannelName>(msg->payload->_simChannel));
		return _callback->forward(nm, hint);
	}

	void OmNetMediator::deliver(message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr msg) {
		_callback->deliver(msg);
	}

	void OmNetMediator::update(const std::string & key, const net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const handle, bool joined) {
		net::NetworkType<net::OMNET>::Key tmp(key);
		_callback->update(tmp, handle, joined);
	}

	std::string OmNetMediator::getSHA1(int channel_name) {
		return _ocallback->getSHA1(channel_name);
	}

	bool OmNetMediator::isResponsibleFor(const net::NetworkType<net::OMNET>::Key & key, const net::NetworkType<net::OMNET>::Key & node) const {
		if (key != node) {
			return false;
		}

		return true;
	}

} /* namespace sim */
} /* namespace m2etis */
