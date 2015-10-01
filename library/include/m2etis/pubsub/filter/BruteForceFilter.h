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

#ifndef __M2ETIS_PUBSUB_FILTER_BRUTEFORCEFILTER_H__
#define __M2ETIS_PUBSUB_FILTER_BRUTEFORCEFILTER_H__

#include <map>
#include <string>

#include "m2etis/message/info/filter/BruteForceFilterInfo.h"
#include "m2etis/pubsub/filter/BaseFilter.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"
#include "m2etis/pubsub/filter/filtervisitors/MatchVisitor.h"

#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"

/**
 * @class BaseFilter
 *
 * This class filters events without optimizations
 * Filter Algorithm used: Brute Force (sequentially comparing all filters to the given event to be published)
 */

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class BruteForceFilter: public BaseFilter<EventType, NetworkType> {
	public:
		typedef message::BruteForceFilterInfo<EventType> FilterInfoType;

		enum {
			size = 0
		};

		/**
		 * \brief Constructor
		 */
		BruteForceFilter() {
		}

		virtual ~BruteForceFilter() {
		}

		virtual void getSubscribePayload(boost::shared_ptr<FilterExp<EventType>> filter, bool is_periodic_resubscribe, typename message::FilterInfo::Ptr filterInfo) override {
			// save filter if not yet stored:
			dynamic_filters_[self_].insert(filter);

			// copy own filters and filters of subscribers into set and return the set:
			auto filters = std::set<boost::shared_ptr<FilterExp<EventType>>>();

			for (auto key_filters_pair : dynamic_filters_) {
				for (auto current_filter : key_filters_pair.second) {
					filters.insert(current_filter);
				}
			}

			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			fInfo->dynamic_filter_ = filters;
			fInfo->isUnsubscribe_ = false;
		}

		virtual void getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo) override {
			dynamic_filters_.clear();

			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			fInfo->dynamic_filter_ = std::set<boost::shared_ptr<FilterExp<EventType>>>();
		}

		virtual typename BaseFilter<EventType, NetworkType>::FilterUnsubscribeInformation getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo, boost::shared_ptr<FilterExp<EventType>> filter) override {
			removeFilter(self_, filter);

			if (dynamic_filters_[self_].empty()) {
				// if last filter has been deregistered, unsubscribe from the tree
				typename FilterInfoType::Ptr fInfo = cast(filterInfo);
				fInfo->dynamic_filter_ = std::set<boost::shared_ptr<FilterExp<EventType>>>();

				return BaseFilter<EventType, NetworkType>::UNSUBSCRIBEFROMTREE;
			}

			bool isFilterFound = false;

			// find out, if a subscriber has registered the same filter:
			for (auto keySubscriptionPair : dynamic_filters_) {
				for (auto registered_filter : keySubscriptionPair.second) {
					if (*registered_filter == *filter) {
						isFilterFound = true;
						break;
					}
				}
			}

			if (isFilterFound) {
				// filter has been deregistered, but a subscriber of this node has registered the same filter
				return BaseFilter<EventType, NetworkType>::CANCELUNSUBSCRIBE;
			} else {
				typename FilterInfoType::Ptr fInfo = cast(filterInfo);
				fInfo->dynamic_filter_ = std::set<boost::shared_ptr<FilterExp<EventType> > >({filter});
				fInfo->isUnsubscribe_ = true;
				return BaseFilter<EventType, NetworkType>::FORWARDUNSUBSCRIBE;
			}
		}

		virtual std::string getPublishPayload(const typename BaseFilter<EventType, NetworkType>::PayloadPtr message_text) const override {
			return "";
		}

		virtual std::string processSubscribePayload(const typename NetworkType::Key & sender_key, typename message::FilterInfo::Ptr filterInfo) override {
			typename FilterInfoType::Ptr fInfo = cast(filterInfo);

			if (fInfo->isUnsubscribe_) {
				// the subscribe message contains the deregistering of a single filter
				// see function getUnsubscribePayload
				// currently only one filter can be contained:
				removeFilter(sender_key, *(fInfo->dynamic_filter_.begin()));
				return "";
			}

			dynamic_filters_[sender_key] = fInfo->dynamic_filter_;

			return "";
		}

		virtual void processUnsubscribePayload(const typename NetworkType::Key & sender_key, typename message::FilterInfo::Ptr filterInfo) override {
			dynamic_filters_.erase(sender_key);
		}

		virtual void processRoutingStrategyUnsubscribeNotification(const typename NetworkType::Key & sender_key) override {
			dynamic_filters_.erase(sender_key);
		}

		virtual bool match(const typename NetworkType::Key & to, typename message::FilterInfo::Ptr filterInfo, typename BaseFilter<EventType, NetworkType>::PayloadPtr event) override {
			for (auto filter_of_recipient : (dynamic_filters_[to])) {
				MatchVisitor<EventType> dynamic_filter_visitor(*event);

				filter_of_recipient->Accept(dynamic_filter_visitor);
				if (dynamic_filter_visitor.get_result()) {
					return true;
				}
			}
			// no matching filter found
			return false;
		}

		virtual bool match(typename BaseFilter<EventType, NetworkType>::PayloadPtr event) override {
			// matching message against all stored predicates
			return match(self_, typename message::FilterInfo::Ptr(), event);
		}

		virtual void setSelf(const typename NetworkType::Key & self) override {
			self_ = self;
		}

	private:
		void removeFilter(const typename NetworkType::Key & key, boost::shared_ptr<FilterExp<EventType> > filter) {
			// comparing objects, not shared_pointers:
			for (auto filter_iter = dynamic_filters_[key].begin(); filter_iter != dynamic_filters_[key].end(); ++filter_iter) {
				if (**filter_iter == *filter) {
					dynamic_filters_[key].erase(filter_iter);
					break;
				}
			}
		}

		typename FilterInfoType::Ptr cast(typename message::FilterInfo::Ptr ptr) const {
			typename FilterInfoType::Ptr ret = boost::dynamic_pointer_cast<FilterInfoType>(ptr);
			if (!ret) {
				M2ETIS_LOG_ERROR("Filter Strategy", "Downcast error of filterInfo");
			}
			return ret;
		}

		std::map<typename NetworkType::Key, std::set<boost::shared_ptr<FilterExp<EventType> > > > dynamic_filters_;

	protected:
		// The own key of this node
		typename NetworkType::Key self_;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_BRUTEFORCEFILTER_H__ */
