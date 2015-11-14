/*
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

/**
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_ORDER_BASEORDER_H__
#define __M2ETIS_PUBSUB_ORDER_BASEORDER_H__

#include "m2etis/message/info/OrderInfo.h"

#include "m2etis/pubsub/ControlTargets.h"
#include "m2etis/pubsub/MsgProcess.h"

namespace m2etis {
namespace pubsub {
namespace order {

	/**
	 * \brief This is the interface that should be used for all Order Strategies
	 * It specifies all functions that shold be overloaded in every strategy as well as some commonly needed functions
	 */
	template<class NetworkType>
	class BaseOrder {
	public:
		BaseOrder(PubSubSystemEnvironment * pssi, bool isRoot) : pssi_(pssi), isRoot_(isRoot), hn(), function_(), sendMsg_() {}
		virtual ~BaseOrder() {}

		/**
		 * \brief used to set the function to be called when a message should be send to the next processing stage
		 * call this function with the id given in receive() to deliver the appropriate message
		 */
		void configureCallback(const boost::function<void(uint64_t, msgProcess)> & func) {
			function_ = func;
		}

		/**
		 * \brief sets the hostname of the current node
		 */
		void setHn(const typename NetworkType::Key & self) {
			hn = self;
		}

		/**
		 * \brief with this function, new messages can be sent directly to nodes
		 */
		void configureSendCallback(const boost::function<void(message::OrderInfo::Ptr, const typename NetworkType::Key &, ControlTarget)> & func) {
			sendMsg_ = func;
		}

		/**
		 * \brief sets whether this node is the root node. Several strategies need this to determind the sequencer
		 */
		void setRoot(bool b) {
			isRoot_ = b;
		}

		/**
		 * \brief returns whether some messages are still waiting
		 */
		virtual bool hasPending() = 0;

		/**
		 * \brief called for every message to be sent
		 */
		virtual bool configureOrderInfo(uint64_t id, const message::ActionType mtype, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & rec) = 0;

		/**
		 * \brief called for every ControlMsg that arrived
		 */
		virtual bool processControlPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief called for every SubscribeMsg that arrived
		 */
		virtual void processSubscribePayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief called for every PublishMsg that arrived
		 */
		virtual void processPublishPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief called for every NotifyMsg that arrived
		 */
		virtual void processNotifyPayload(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief called for every message that is ready to be delivered
		 * \param[in] id Id to reference the message later
		 * \param[in] ptr Pointer to the OrderStruct of the message
		 * \param[in] sender containts the sender of the message
		 */
		virtual void receive(uint64_t id, typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief notified when message dropped by filter or validity strategy
		 */
		virtual void notifyRemovedMessage(typename message::OrderInfo::Ptr ptr, const typename NetworkType::Key & receiver) = 0;

	protected:
		/**
		 * \brief
		 */
		PubSubSystemEnvironment * pssi_;

		/**
		 * \brief true if this node is the root node
		 */
		bool isRoot_;

		/**
		 * \brief hostname of the node
		 */
		typename NetworkType::Key hn;

		/**
		 * \brief stores the function to be called for delivering
		 * 1. Parameter (int): specifies the message id that should be further processed
		 */
		boost::function<void(uint64_t, msgProcess)> function_;

		/**
		 * \brief stores the function to send new messages
		 * This function will send a newly created control message containing the given OrderStruct to the specified sender.
		 * The Tree ensures that this message will be sent to the specified receiver and will not be processed by any other strategie.
		 * 1. Parameter (OrderInfo::Ptr): Pointer to the OrderStruct to be used
		 * 2. Parameter (string): Name of the target. This can either be a real node identifier or
		 * "root" to send directly to the root of this tree
		 */
		boost::function<void(message::OrderInfo::Ptr, const typename NetworkType::Key &, ControlTarget)> sendMsg_;
	};

} /* namespace order */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ORDER_BASEORDER_H__ */

/**
 *  @}
 */
