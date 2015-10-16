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

#ifndef __M2ETIS_PUBSUB_ROUTING_BASEROUTING_H__
#define __M2ETIS_PUBSUB_ROUTING_BASEROUTING_H__

#include "m2etis/parameters/RoutingParameters.h"
#include "m2etis/pubsub/ControlTargets.h"

/**
 * @class BaseRouting
 * @author Hristiyan Pehlivanov
 *
 * This abstract class defines the functions which have to be implemented by every routing strategy. This way Channel can use all routing  * strategies with the same interface.
 */

namespace m2etis {
namespace pubsub {
namespace routing {

	template<class NetworkType>
	class BaseRouting {
		typedef std::vector<typename NetworkType::Key> KeyList;

	public:
		BaseRouting(unsigned int, PubSubSystemEnvironment *) : sendCtrlMsg_() {
		}

		virtual ~BaseRouting() {
		}

		/**
		 * \brief with this function, new messages can be sent directly to nodes
		 */
		void configureSendCallback(const boost::function<void(typename message::RoutingInfo<NetworkType>::Ptr, const typename NetworkType::Key &, ControlTarget)> & func) {
			sendCtrlMsg_ = func;
		}

		/**
		 * Handles a subscribe message. The routing information of this node is updated, a decision about continuing the workflow is made
		 * and saved in a routingInfo object together with the necessary information.
		 *
		 * @param routingInfo contains routing information provided, will be updated with routing decisions
		 * @param sender the senders's key
		 *
		 */
		virtual bool processSubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) = 0;

		/**
		 * Handles an unsubscribe message. The routing information of this node is updated, a decision about continuing the workflow is
		 * made and saved in a routingInfo object together with the necessary information.
		 *
		 * @param routingInfo contains routing information provided by the sender, will be updated
		 * @param sender the senders's key
		 *
		 */
		virtual void processUnsubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) = 0;

		/**
		 * Handles a publish message. Most strategies do not do anything, however such messages could affect the tree structure and every
		 * strategy should choose on its own what to do.
		 *
		 * @param routingInfo contains routing information provided by the sender, will be updated
		 * @param sender the senders's key
		 */
		virtual void processPublishPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) = 0;

		/**
		 * Handles a notify message. Most strategies do not do anything, however such messages could affect the tree structure and every
		 * strategy should choose on its own what to do.
		 *
		 * @param routingInfo contains routing information provided by the sender, will be updated
		 * @param sender the senders's key
		 */
		virtual void processNotifyPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) = 0;

		/**
		 * Handles a control message. The routing information of this node is updated, a decision about continuing the workflow is made and
		 * saved in a routingInfo object together with the necessary information. Control messages are used by routing strategies, that do
		 * not build the tree implicitly with subscribe messages, but need more complex control commands.
		 *
		 * @param routingInfo contains routing information provided by the sender, will be updated with routing decisions
		 * @param sender the senders's key
		 */
		virtual void processControlPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) = 0;

		/**
		 * Returns a list of target nodes for the specified message type and action coomand in regard to the node's
		 * position in the logic distribution tree.
		 *
		 * @param mtype the type of the message (SUBSCRIBE, UNSUBSCIBE, CONTROL or PUBLISH)
		 * @param routingInfo contains the action command and sometimes possible target nodes
		 */
		virtual KeyList getTargetNodes(const message::ActionType mtype, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key & receiver) const = 0;

		/**
		 * Configures the periodic messages. Every node runs a thread that triggers subscribe messages. These messages are used for
		 * periodical tasks like heart beat, checking the cluster size etc. This means that the message is not always from type subscribe
		 * or does not have the same subscription information. Every strategy chooses how to use the periodic messages and can configure it
		 * in this function.
		 * @param mtype Type of message to configure for
		 * @param routingInfo object to be configured with the necessary routing information
		 */
		virtual void configureRoutingInfo(message::ActionType & msgType, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key & receiver) = 0;

		virtual void setUnsubscriptionListener(const boost::function<void(const typename NetworkType::Key)> & listener) = 0;

	protected:
		boost::function<void(typename message::RoutingInfo<NetworkType>::Ptr, typename NetworkType::Key, ControlTarget)> sendCtrlMsg_;
	};

} /* namespace routing */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ROUTING_BASEROUTING_H__ */
