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

/**
 * \addtogroup simulator
 * @{
 */

#ifndef __M2ETIS_SIM_PUBSUBWRAPPER_H__
#define __M2ETIS_SIM_PUBSUBWRAPPER_H__

#include <string>

#include "m2etis/message/M2Message.h"
#include "m2etis/message/NetworkMessage.h"
#include "m2etis/pubsub/ChannelInterface.h"
#include "m2etis/pubsub/DeliverCallbackInterface.h"
#include "m2etis/pubsub/config/ChannelName.h"

#include "m2etis/sim/OmNetKeyProvider.h"

namespace m2etis {
namespace pubsub {
	class PubSubSystem;
	class PubSubSystemEnvironment;
} /* namespace pubsub */
namespace sim {

	class OmNetCallbackInterface;

	class PubSubWrapper {
	public:
		/**
		 * \brief constructor creating wrapper out of simulator
		 *
		 * \param[in] o callback in simulator
		 * \param[in] ip IP of the node
		 * \param[in] port port of the node
		 * \param[in] known_hostname IP of the root node
		 * \param[in] known_hostport port of the root node
		 */
		PubSubWrapper(OmNetCallbackInterface * o, const std::string & ip, const int port, const std::string & known_hostname, const int known_hostport, const std::vector<std::string> & rootList);

		/**
		 * \brief wrapper for subscribe method of PubSubSystem
		 *
		 * \param[in] channel channel to subscribe on
		 * \param[in] callback DeliverCallback to be registered for this channel
		 */
		void subscribe(const pubsub::ChannelName channel, pubsub::BasicDeliverCallbackInterface<SimulationEventType> & callback);

		/**
		 * \brief wrapper for publish method of PubSubSystem
		 *
		 * \param[in] channel channel to publish on
		 * \param[in] msg message to be published
		 */
		void publish(const pubsub::ChannelName channel, const message::M2Message<SimulationEventType>::Ptr msg) const;

		/**
		 * \brief wrapper for unsubscribe method of PubSubSystem
		 *
		 * \param[in] channel channel to unsubscribe off
		 */
		void unsubscribe(const pubsub::ChannelName channel) const;

		/**
		 * \brief method to create a message for the specified channel with the given payload
		 *
		 * \param[in] channel channel for which the message will be created
		 * \param[in] v payload of the new message
		 */
		message::M2Message<SimulationEventType>::Ptr createMessage(const pubsub::ChannelName channel, const SimulationEventType & v);

		/**
		 * \brief method to create NetworkMessage out of a M2Message
		 *
		 * \param[in] msg message to be transformed
		 * \param[in] chan channel for this message
		 */
		message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr transformToNetworkMessage(message::M2Message<SimulationEventType>::Ptr msg, const pubsub::ChannelName chan);

		/**
		 * \brief method to create M2Message out of a NetworkMessage
		 *
		 * \param[in] msg message to be transformed
		 * \param[in] chan channel for this message
		 */
		message::M2Message<SimulationEventType>::Ptr transformToM2etisMessage(message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr msg, const pubsub::ChannelName chan);

		/**
		 * \brief getter for PubSubSystemEnvironment
		 */
		pubsub::PubSubSystemEnvironment * pssi();

		/**
		 * \brief method updating Scheduler for simulator
		 */
		void Update();

		/**
		 * \brief PubSubSystem being wrapped by this class
		 */
		pubsub::PubSubSystem * _pubsub;
	};

} /* namespace sim */
} /* namespace m2etis */

#endif /* __M2ETIS_SIM_PUBSUBWRAPPER_H__ */

/**
 *  @}
 */
