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
 * @ {
 */

#ifndef __M2ETIS_SIM_OMNETMEDIATOR_H__
#define __M2ETIS_SIM_OMNETMEDIATOR_H__

#include <string>

#include "m2etis/config/GeneratedEventTypes.h"

#include "m2etis/message/MessageType.h"
#include "m2etis/message/NetworkMessage.h"

#include "m2etis/net/NetworkInterface.h"
#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkType.h"
#include "m2etis/net/NodeHandle.h"

#include "m2etis/sim/OmNetCallbackInterface.h"

namespace m2etis {
namespace sim {

	class PubSubWrapper;

	class OmNetMediator : public net::NetworkInterface<net::NetworkType<net::OMNET> > {
	public:
		/**
		 * \brief constructor
		 *
		 * \param[in] ip local IP of the node
		 * \param[in] port local port of the node
		 * \param[in] known_hostname IP of the root node
		 * \param[in] known_hostport port of the root node
		 */
		OmNetMediator(const std::string & ip, const int port, const std::string & known_hostname, const int known_hostport) : _nodeHandle(new net::NodeHandle<net::NetworkType<net::OMNET>>), _callback(NULL), _ocallback(NULL), _name(ip), _hostName(known_hostname), _listenPort(port), _hostPort(known_hostport), _root(_hostName + std::string(":") + std::to_string(_hostPort)), _pubsub() {
		}

		/**
		 * \brief destructor
		 */
		~OmNetMediator() {}

		/**
		 * \brief method to register M2etisAdapter in here
		 *
		 * \param[in] o the base class of the M2etisAdapter known in m2etis library
		 */
		void registerOmNetCallback(OmNetCallbackInterface * o);

		/**
		 * \brief sets callback of NetworkController to communicate with Tree
		 *
		 * \param[in] cb pointer to the NetworkController
		 */
		void setCallback(net::NetworkCallbackInterface<net::NetworkType<net::OMNET>> * cb);

		/**
		 * \brief method to send the given message into the M2etisAdapter
		 *
		 * \param[in] type type of the message
		 * \param[in] to receiver of this message
		 * \param[in] payload the message to be sent
		 * \param[in] hint NodeHandle
		 * \param[in] size the size of the serialized message, needed for metric in M2etisAdapter
		 */
		void send(const message::ActionType type, const std::string & to, const message::M2Message<SimulationEventType>::Ptr payload, net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const hint, unsigned int size);

		/**
		 * \brief method called by NetworkController to send message into the network
		 *
		 * \param[in] msg message to be sent
		 * \param[in] hint NodeHandle
		 */
		void send(const net::NetworkType<net::OMNET>::Message::Ptr msg, net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const hint);

		/**
		 * \brief method called by NetworkController to send message into the network
		 *
		 * \param[in] msg message to be sent
		 */
		void send(const net::NetworkType<net::OMNET>::Message::Ptr msg);

		/**
		 * \brief returns this nodes NodeHandle
		 */
		net::NodeHandle<net::NetworkType<net::OMNET> >::Ptr getSelfNodeHandle() const;

		/**
		 * \brief not used
		 */
		void registerMessageType(const message::ActionType type, const bool ack) const;

		/**
		 * \brief not used
		 */
		void registerMessageType(const net::NetworkType<net::OMNET>::MessageType type, const bool ack) const;

		/**
		 * \brief method to forward message using an overlay
		 *
		 * \params[in] key key of a node
		 * \params[in] msg message being forwarded
		 * \params[in] hint NodeHandle
		 */
		bool forward(std::string & key, message::M2Message<SimulationEventType>::Ptr msg, const net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr hint);

		/**
		 * \brief method to deliver messages from M2etisAdapter
		 *
		 * \param[in] msg message received by M2etisAdapter
		 */
		void deliver(message::NetworkMessage<net::NetworkType<net::OMNET>>::Ptr msg);

		/**
		 * \brief method called by M2etisAdapter to validate status in overlay
		 *
		 * \param[in] key key of the not being validated
		 * \param[in] handle NodeHandle
		 * \param[in] joined flag whether the node joined or leaved
		 */
		void update(const std::string & key, const net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const handle, bool joined);

		/**
		 * \brief method to get SHA1 key out of channel name
		 *
		 * \param[in] channel_name the topic for which the SHA1 key will be generated
		 */
		std::string getSHA1(int channel_name);

		/**
		 * \brief returns whether one node is responsible for another
		 *
		 * \param[in] key the node being checked whether it is responsible
		 * \param[in] node the node being the checked whether it isn't responsible
		 */
		bool isResponsibleFor(const net::NetworkType<net::OMNET>::Key & key, const net::NetworkType<net::OMNET>::Key & node) const;

		/**
		 * \brief returns the root node of the tree
		 */
		net::NetworkType<net::OMNET>::Key getRoot() const { return _root; }

		/**
		 * \brief registers the PubSubWrapper of this simulation node in the wrapper
		 *
		 * \param[in] p the pointer to the PubSubWrapper being registered
		 */
		void registerWrapper(PubSubWrapper * p) {
			_pubsub = p;
		}

	private:
		net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr _nodeHandle;
		net::NetworkCallbackInterface<net::NetworkType<net::OMNET>> * _callback;
		OmNetCallbackInterface * _ocallback;
		std::string _name;
		std::string _hostName;
		int _listenPort;
		int _hostPort;
		net::NetworkType<net::OMNET>::Key _root;
		PubSubWrapper * _pubsub;

		OmNetMediator(const OmNetMediator &);
		OmNetMediator & operator=(const OmNetMediator &);
	};

} /* namespace sim */
} /* namespace m2etis */

#endif /* __M2ETIS_SIM_OMNETMEDIATOR_H__ */

/**
 *  @}
 */
