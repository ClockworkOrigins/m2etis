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


#ifndef __M2ETIS_PUBSUB_FILTER_NULLFILTER_H__
#define __M2ETIS_PUBSUB_FILTER_NULLFILTER_H__

#include <string>

#include "m2etis/message/info/filter/NullFilterInfo.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace pubsub {
namespace filter {

	// registering a filter means to get the same number of events or more than before
	// deregistering a filter means to get the same number of events or less than before

	template<typename EventType, typename NetworkType>
	class NullFilter {
	public:
		typedef boost::shared_ptr<EventType> PayloadPtr;
		typedef message::NullFilterInfo<EventType> FilterInfoType;

		enum { size = 0 };

		// return type for getUnsubscribePayload:
		enum FilterUnsubscribeInformation { // TODO: (Roland) maybe move to BaseFiltering base class
			FORWARDUNSUBSCRIBE,
			UNSUBSCRIBEFROMTREE, 	// if last filter has been deregistered, unsubscribe from the tree
			CANCELUNSUBSCRIBE    	// filter has been deregsitered, but a subscriber of this node has registered
									// the same filter, so the unsubscribe message must not be forwarded (if the filter strategy can detect this,
									// otherweise forward unsubscribe message with an appropriate filter info
		};

		NullFilter() {}
		virtual ~NullFilter() {}

		void getSubscribePayload(boost::shared_ptr<FilterExp<EventType>>, bool, typename message::FilterInfo::Ptr) {  // return boost::make_shared<m2etis::pubsub::filter::FilterExp<EventType> > (m2etis::pubsub::filter::TruePredicate<EventType>()); // return the predicate, it must be sent "up" and save it yourself
			return;
		}

		void getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo) {
			// perhaps send it "up" that it can be removed?
			// return boost::make_shared<m2etis::pubsub::filter::FilterExp<EventType> > ();
		}

		FilterUnsubscribeInformation getUnsubscribePayload(typename message::FilterInfo::Ptr, boost::shared_ptr<FilterExp<EventType>>) {
			return FORWARDUNSUBSCRIBE;
		}

		std::string getPublishPayload(const PayloadPtr message_text) const {
			return ""; // prefilter or preprocess the message and give other nodes a hint
		}

		std::string processSubscribePayload(const typename NetworkType::Key &, typename message::FilterInfo::Ptr) {
			return ""; // from sender, comes that filterinfo (maybe a predicate)
		}

		bool processUnsubscribePayload(const typename NetworkType::Key &, typename message::FilterInfo::Ptr) {
			return ""; // from sender, comes that filterinfo (maybe a predicate)
		}

		// called by routing strategy:
		void processRoutingStrategyUnsubscribeNotification(const typename NetworkType::Key) {
		}

		bool match(const typename NetworkType::Key &, typename message::FilterInfo::Ptr, PayloadPtr) const {
			return true; 	// Given the filterinfo (from getPublishPayload), the message and the recipient decide if he need's that message
		}

		// function matching against own dynamic_filters before delivering to application:
		bool match(PayloadPtr) const {
			return true;
		}

		void setSelf(const typename NetworkType::Key &) {}

	private:
		typename FilterInfoType::Ptr cast(typename message::FilterInfo::Ptr ptr) const {
			typename FilterInfoType::Ptr ret = boost::dynamic_pointer_cast<FilterInfoType>(ptr);
			if (!ret) {
				M2ETIS_LOG_ERROR("Filter Strategy", "Downcast error of filterInfo");
			}
			return ret;
          }
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_NULLFILTER_H__ */
