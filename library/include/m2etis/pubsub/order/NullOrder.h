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

#ifndef __M2ETIS_PUBSUB_ORDER_NULLORDER_H__
#define __M2ETIS_PUBSUB_ORDER_NULLORDER_H__

#include "m2etis/message/info/order/NullOrderInfo.h"
#include "m2etis/pubsub/order/BaseOrder.h"

namespace m2etis {
namespace pubsub {
namespace order {

	/**
	 * \brief This class implements the NullOrder Strategy
	 * This Strategy does nothing for ordering. All Messages are forwarded as fast as possible
	 */
	template<class NetworkType>
	class NullOrder : public BaseOrder<NetworkType> {
	public:
		typedef message::NullOrderInfo OrderInfoType;

		NullOrder(PubSubSystemEnvironment * pssi, bool b) : BaseOrder<NetworkType>(pssi, b) {}
		virtual ~NullOrder() {}

		/**
		 * \brief there are never pending messages
		 */
		bool hasPending() {
			return false;
		}

		/**
		 * \brief no extra informations need to be stored
		 */
		bool configureOrderInfo(uint64_t id, const message::ActionType, typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
			// no infos, just send
			BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
			return true;
		}

		/**
		 * \brief nothing has to be done on arrival
		 */
		void processSubscribePayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief nothing has to be done on arrival
		 */
		void processPublishPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief called for every NotifyMsg that arrived
		 */
		void processNotifyPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}

		/**
		 * \brief nothing has to be done on arrival
		 */
		bool processControlPayload(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
			return false;
		}

		/**
		 * \brief this function is used to get the pointers to all other NullOrder instances of this Channel (other trees)
		 * They are needed for multi-group ordering
		 */
		void otherOrders(const std::vector<NullOrder *> &) {
		}

		/**
		 * \brief deliver the message as soon as it arrived
		 */
		void receive(uint64_t id, typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
			// deliver instant
			BaseOrder<NetworkType>::function_(id, msgProcess::MSG_PROCESS_AND_DELETE);
		}

		/**
		 * \brief notified when message dropped by filter or validity strategy
		 */
		void notifyRemovedMessage(typename message::OrderInfo::Ptr, const typename NetworkType::Key &) {
		}
	};

} /* namespace order */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ORDER_NULLORDER_H__ */
