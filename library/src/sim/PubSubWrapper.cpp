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

#include "m2etis/sim/PubSubWrapper.h"

#include "m2etis/net/NetworkFactory.h"
#include "m2etis/pubsub/PubSubSystem.h"
#include "m2etis/pubsub/PubSubSystemEnvironment.h"
#include "m2etis/sim/OmNetMediator.h"

namespace m2etis {
namespace sim {

	PubSubWrapper::PubSubWrapper(OmNetCallbackInterface * o, const std::string & ip, const int port, const std::string & known_hostname, const int known_hostport, const std::vector<std::string> & rootList) : _pubsub(new pubsub::PubSubSystem(ip, port, known_hostname, known_hostport, rootList)) {
		dynamic_cast<OmNetMediator *>(pssi()->_factory->createNetworkController(net::NetworkType<net::OMNET>())->network_)->registerOmNetCallback(o);

		pssi()->clock_.setSim(o);
	}

	void PubSubWrapper::subscribe(const pubsub::ChannelName channel, pubsub::BasicDeliverCallbackInterface<SimulationEventType> & callback) {
		_pubsub->subscribe<SimulationEventType>(channel, callback);
	}

	void PubSubWrapper::publish(const pubsub::ChannelName channel, const message::M2Message<SimulationEventType>::Ptr msg) const {
		_pubsub->publish<SimulationEventType>(channel, msg);
	}

	void PubSubWrapper::unsubscribe(const pubsub::ChannelName channel) const {
		_pubsub->unsubscribe<SimulationEventType>(channel);
	}

	message::M2Message<SimulationEventType>::Ptr PubSubWrapper::createMessage(const pubsub::ChannelName channel, const SimulationEventType & v) {
		return _pubsub->createMessage<SimulationEventType>(channel, v);
	}

	message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr PubSubWrapper::transformToNetworkMessage(message::M2Message<SimulationEventType>::Ptr msg, const pubsub::ChannelName chan) {
		message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr m = _pubsub->getChannelHandle<SimulationEventType>(chan)->transformToNetworkMessage(msg);
		m->pl = *msg->payload.get();
		return m;
	}

	message::M2Message<SimulationEventType>::Ptr PubSubWrapper::transformToM2etisMessage(message::NetworkMessage<net::NetworkType<net::OMNET> >::Ptr msg, const pubsub::ChannelName chan) {
		return _pubsub->getChannelHandle<SimulationEventType>(chan)->transformToM2Message(msg);
	}

	void PubSubWrapper::Update() {
		pssi()->clock_.Update();
		pssi()->scheduler_.workerSim();
	}

	pubsub::PubSubSystemEnvironment * PubSubWrapper::pssi() {
		return _pubsub->_pssi;
	}

} /* namespace pubsub */
} /* namespace m2etis */
