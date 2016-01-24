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
 * \addtogroup pubsub
 * @{
 */

#ifndef __M2ETIS_PUBSUB_DELIVER_BASEDELIVER_H__
#define __M2ETIS_PUBSUB_DELIVER_BASEDELIVER_H__

#include <string>

#include "m2etis/pubsub/ControlTargets.h"
#include "m2etis/pubsub/MsgProcess.h"

namespace m2etis {
namespace pubsub {
namespace deliver {

	enum class Amount {
		EXACTLY_ONCE,
		AT_MOST_ONCE,
		AT_LEAST_ONCE
	};
	
	template<class NetworkType>
	class BaseDeliver {
	public:
		explicit BaseDeliver(PubSubSystemEnvironment * pssi) : pssi_(pssi), process_(), sendCtrlMsg_() {}

		virtual ~BaseDeliver() {}

		/**
		 * \brief used to set the function to be called when a message should be send to the next processing stage
		 * call this function with the id given in receive() to deliver the appropriate message
		 */
		void configureCallback(const boost::function<void(uint64_t, msgProcess)> & func) {
			process_ = func;
		}

		/**
		 * \brief with this function, new messages can be sent directly to nodes
		 */
		void configureSendCallback(const boost::function<void(message::DeliverInfo::Ptr, const typename NetworkType::Key &, ControlTarget)> & func) {
			sendCtrlMsg_ = func;
		}

		/**
		 * \brief creates the DeliverInfo
		 */
		virtual void configureDeliverInfo(uint64_t id, const message::ActionType mtype, typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & receiver, message::ControlType ct) = 0;

		/**
		 * \brief creates the DeliverInfo
		 */
		virtual bool processPublishPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief creates the DeliverInfo
		 */
		virtual bool processOtherControlPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief creates the DeliverInfo
		 */
		virtual bool processControlPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief creates the DeliverInfo
		 */
		virtual bool processNotifyPayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief creates the DeliverInfo
		 */
		virtual bool processSubscribePayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

		/**
		 * \brief processes Control Messages (id got acked)
		 */
		virtual bool processUnsubscribePayload(typename message::DeliverInfo::Ptr ptr, const typename NetworkType::Key & sender) = 0;

	protected:
		/**
		 * \brief PubSubSystemEnvironment
		 */
		PubSubSystemEnvironment * pssi_;

		/**
		* \brief stores the function to be called for delivering
		* 1. Parameter (int): specifies the message id that should be further processed
		*/
		boost::function<void(uint64_t, msgProcess)> process_;

		/**
		* \brief stores the function to send new messages
		* This function will send a newly created control message containing the given DeliverStruct to the specified sender.
		* The Tree ensures that this message will be sent to the specified receiver and will not be processed by any other strategie.
		* 1. Parameter (DeliverInfo::Ptr): Pointer to the DeliverStruct to be used
		* 2. Parameter (string): Name of the target. This can either be a real node identifier or
		* "root" to send directly to the root of this tree
		*/
		boost::function<void(message::DeliverInfo::Ptr, typename NetworkType::Key, ControlTarget)> sendCtrlMsg_;
	};

} /* namespace deliver */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_DELIVER_BASEDELIVER_H__ */

/**
 * @}
 */
