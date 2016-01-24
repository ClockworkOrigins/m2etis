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

#ifndef __M2ETIS_PUBSUB_FILTER_BASEFILTER_H__
#define __M2ETIS_PUBSUB_FILTER_BASEFILTER_H__

#include <string>

#include "m2etis/message/info/FilterInfo.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

#include "boost/shared_ptr.hpp"

/**
 * @class BaseFilter
 *
 * This abstract class defines the functions which have to be implemented by every filter strategy. This way Channel can use all filter  strategies with the same interface.
 *
 * Using filters in general usually assumes the subscriptions to be rather static, i.e.
 * filters are not registered and deregistered at a high frequency
 */

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class BaseFilter {
	public:
		typedef boost::shared_ptr<EventType> PayloadPtr;
		BaseFilter() {
		}
		virtual ~BaseFilter() {
		}

		// return type for getUnsubscribePayload:
		enum FilterUnsubscribeInformation {
			FORWARDUNSUBSCRIBE,
			UNSUBSCRIBEFROMTREE, // if last filter has been deregistered, unsubscribe from the tree
			CANCELUNSUBSCRIBE // filter has been deregsitered, but a subscriber of this node has registered
		// the same filter, so the unsubscribe message must not be forwarded (if the filter strategy can detect this,
		// otherwise forward unsubscribe message with an appropriate filter info)
		};

		/**
		 * \brief processes the new filter
		 */
		virtual void getSubscribePayload(boost::shared_ptr<FilterExp<EventType> > filter, bool is_periodic_resubscribe, typename message::FilterInfo::Ptr filterInfo) = 0;

		/**
		 * \brief removes all filters
		 */
		virtual void getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo) = 0;

		/**
		 * \brief deregisters individual filter
		 * returns information whether to
		 * - unsubscribe from the tree (if last filter has been deregistered)
		 *
		 *   and whether
		 *
		 * - the unsubscribe message has to be forwarded (only if no subscriber of this node has
		 *    registered the same filter)
		 */
		virtual FilterUnsubscribeInformation getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo, boost::shared_ptr<FilterExp<EventType> > filter) = 0;

		/**
		 * \brief prefilter or preprocess the message and give other nodes a hint (currently not used)
		 */
		virtual std::string getPublishPayload(const PayloadPtr message_text) const = 0;

		/**
		 * \brief processes the set of received filters from sender
		 */
		virtual std::string processSubscribePayload(const typename NetworkType::Key & sender_key, typename message::FilterInfo::Ptr filterInfo) = 0;

		/**
		 * \brief removes all filters of the sender with the key given
		 */
		virtual void processUnsubscribePayload(const typename NetworkType::Key & sender_key, typename message::FilterInfo::Ptr filterInfo) = 0;

		/**
		 * \brief removes all filters of the sender with the key given
		 * intended for routing strategies to signal purged subscribers
		 */
		virtual void processRoutingStrategyUnsubscribeNotification(const typename NetworkType::Key & sender_key) = 0;

		/**
		 * \brief matches against dynamic filters of subscriber
		 * call this function to filter before forwarding event to subscriber
		 */
		virtual bool match(const typename NetworkType::Key & to, typename message::FilterInfo::Ptr filterInfo, PayloadPtr event) = 0;

		/**
		 * \brief matches against own dynamic_filters
		 * call this function to filter before delivering to application
		 */
		virtual bool match(PayloadPtr event) = 0;

		virtual void setSelf(const typename NetworkType::Key & self) = 0;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_BASEFILTER_H__ */

/**
 *  @}
 */
