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

#ifndef __M2ETIS_PUBSUB_DELIVER_NULLDELIVER_H__
#define __M2ETIS_PUBSUB_DELIVER_NULLDELIVER_H__

#include <string>

#include "m2etis/message/info/deliver/NullDeliverInfo.h"

#include "m2etis/pubsub/MsgProcess.h"
#include "m2etis/pubsub/deliver/BaseDeliver.h"

namespace m2etis {
namespace pubsub {
namespace deliver {

	template<class NetworkType>
	class NullDeliver : public BaseDeliver<NetworkType> {
	public:
		typedef message::NullDeliverInfo DeliverInfoType;

		NullDeliver(PubSubSystemEnvironment * pssi, const typename NetworkType::Key &) : BaseDeliver<NetworkType>(pssi) {
		}

		virtual ~NullDeliver() {}

		/**
		 * \brief used to process payload
		 */
		bool processPublishPayload(typename message::DeliverInfo::Ptr, const typename NetworkType::Key &) {
			return true;
		}

		/**
		 * \brief used to process payload
		 */
		bool processNotifyPayload(typename message::DeliverInfo::Ptr, const typename NetworkType::Key &) {
			return true;
		}

		/**
		 * \brief used to process payload
		 */
		bool processSubscribePayload(typename message::DeliverInfo::Ptr, const typename NetworkType::Key &) {
			return true;
		}

		/**
		 * \brief used to process payload
		 */
		bool processUnsubscribePayload(typename message::DeliverInfo::Ptr, const typename NetworkType::Key &) {
			return true;
		}

		bool processOtherControlPayload(typename message::DeliverInfo::Ptr, const typename NetworkType::Key &) {
			return true;
		}

		/**
		 * \brief creates the DeliverInfo
		 */
		void configureDeliverInfo(uint64_t id, const message::ActionType, typename message::DeliverInfo::Ptr, const typename NetworkType::Key &, message::ControlType) {
			BaseDeliver<NetworkType>::process_(id, msgProcess::MSG_PROCESS_AND_DELETE);
		}

		/**
		 * \brief processes Control Messages (id got acked)
		 */
		bool processControlPayload(typename message::DeliverInfo::Ptr, const typename NetworkType::Key &) {
			return false;
		}
	};

} /* namespace deliver */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_DELIVER_NULLDELIVER_H__ */

/**
 *  @}
 */
