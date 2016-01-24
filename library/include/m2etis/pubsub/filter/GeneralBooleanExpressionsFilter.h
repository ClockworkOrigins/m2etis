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

#ifndef __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANEXPRESSIONSFILTER_H__
#define __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANEXPRESSIONSFILTER_H__

#include <map>
#include <set>
#include <string>
#include <vector>

#include "m2etis/message/info/filter/GeneralBooleanExpressionsFilterInfo.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/PredicateIndex.h"
#include "m2etis/pubsub/filter/filtervisitors/GeneralBooleanExpressionsPreProcessVisitor.h"
#include "m2etis/pubsub/filter/filtervisitors/GetMinPredicatesVisitor.h"
#include "m2etis/pubsub/filter/filtervisitors/MatchVisitor.h"

#include "boost/tuple/tuple.hpp"

/**
 * \class GeneralBooleanExpressionsFilter
 *
 * This class filters events by matching with arbitrary boolean expressions based on Bittner´s algorithm
 *
 *
 * Current Restrictions:
 *
 * - Only AttributeFilers  (including subtypes) as Predicates allowed, arbitrary Predicates are not supported
 *
 * - No predicates with empty constant vectors allowed
 *
 */


namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class GeneralBooleanExpressionsFilter : public BaseFilter<EventType, NetworkType> {
	public:
		typedef message::GeneralBooleanExpressionsFilterInfo<EventType> FilterInfoType;

		enum {
			size = 0
		};

		GeneralBooleanExpressionsFilter() {
		}

		virtual ~GeneralBooleanExpressionsFilter() {
		}

		virtual void getSubscribePayload(boost::shared_ptr<FilterExp<EventType>> filter, bool, typename message::FilterInfo::Ptr filterInfo) override {
			// insert into index data structures:
			addSubscription(self_, filter);

			// copy own filters and filters of subscribers into set and forward the set:
			auto filters = std::set<boost::shared_ptr<FilterExp<EventType>>>();

			for (auto subscription : subscriptions_) {
				filters.insert(boost::get<1>(subscription.second));
			}

			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			fInfo->dynamic_filter_ = filters;
			fInfo->isUnsubscribe_ = false;
		}

		virtual void getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo) override {
			// clear all indexes:
			for (auto predicateID_subscription_pair : predicate_subscription_association_table_) {
				predicate_identifier_factory_.freeID(predicateID_subscription_pair.first);
			}

			for (auto subscriptionID_subscription_pair : subscription_subscriber_association_table) {
				subscription_identifier_factory_.freeID(subscriptionID_subscription_pair.first);
			}

			has_new_subscription_ = true;

			subscription_subscriptionID_association_table.clear();
			subscription_subscriber_association_table.clear();
			subscriptions_.clear();
			predicate_subscription_association_table_.clear();
			predicate_indexes_.clear();

			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			fInfo->dynamic_filter_ = std::set<boost::shared_ptr<FilterExp<EventType>>>();
		}

		virtual typename BaseFilter<EventType, NetworkType>::FilterUnsubscribeInformation getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo, boost::shared_ptr<FilterExp<EventType>> filter) override {
			bool hasMoreSubscribers;
			bool hasMoreSubscriptions;

			removeSubscription(self_, filter, &hasMoreSubscribers, &hasMoreSubscriptions);

			if (!hasMoreSubscriptions) {
				// if last filter has been deregistered, unsubscribe from the tree
				typename FilterInfoType::Ptr fInfo = cast(filterInfo);
				fInfo->dynamic_filter_ = std::set<boost::shared_ptr<FilterExp<EventType>>>();

				return BaseFilter<EventType, NetworkType>::UNSUBSCRIBEFROMTREE;
			}

			if (hasMoreSubscribers) {
				// filter has been deregsitered, but a subscriber of this node has registered the same filter
				return BaseFilter<EventType, NetworkType>::CANCELUNSUBSCRIBE;
			} else {
				typename FilterInfoType::Ptr fInfo = cast(filterInfo);
				fInfo->dynamic_filter_ = std::set<boost::shared_ptr<FilterExp<EventType>>>({filter});
				fInfo->isUnsubscribe_ = true;
				return BaseFilter<EventType, NetworkType>::FORWARDUNSUBSCRIBE;
			}
		}

		virtual std::string getPublishPayload(const typename BaseFilter<EventType, NetworkType>::PayloadPtr message_text) const override {
			return "";
		}

		virtual std::string processSubscribePayload(const typename NetworkType::Key & sender, typename message::FilterInfo::Ptr filterInfo) override {
			typename FilterInfoType::Ptr fInfo = cast(filterInfo);

			bool hasMoreSubscribers;
			bool hasMoreSubscriptions;

			if (fInfo->isUnsubscribe_) {
				// currently only one filter can be contained:
				removeSubscription(sender, *(fInfo->dynamic_filter_.begin()), &hasMoreSubscribers, &hasMoreSubscriptions);

				return "";
			}

			for (auto filter : fInfo->dynamic_filter_) {
				addSubscription(sender, filter);
			}

			// remove all filters not contained in the filterinfo, but in the indexes:

			bool is_filter_found = false;
			auto erasable_subscriptions = std::vector<boost::shared_ptr<FilterExp<EventType>>>();

			for (auto filter_filterID_pair : subscription_subscriptionID_association_table) {
				for (auto filter_ptr : fInfo->dynamic_filter_) {
					if (*(filter_filterID_pair.first) == *filter_ptr) {
						is_filter_found = true;

						break;
					}
				}
				if (!is_filter_found) {
					erasable_subscriptions.push_back(filter_filterID_pair.first);
					// may not be removed here to avoid invalidating iterators
				}
				is_filter_found = false;
			}

			for (auto erasable_subscription : erasable_subscriptions) {
				removeSubscription(sender, erasable_subscription, &hasMoreSubscribers, &hasMoreSubscriptions);
			}

			return "";
		} // ProcessSubscribePayload

		virtual void processUnsubscribePayload(const typename NetworkType::Key & sender, typename message::FilterInfo::Ptr filterInfo) override  {
			bool hasMoreSubscribers;
			bool hasMoreSubscriptions;

			// remove all filters of sender

			for (auto filter_filterID_pair : subscription_subscriptionID_association_table) {
				removeSubscription(sender, (filter_filterID_pair.first), &hasMoreSubscribers, &hasMoreSubscriptions);
			}
		}

		// called by routing strategy:
		virtual void processRoutingStrategyUnsubscribeNotification(const typename NetworkType::Key & sender) override {
			bool hasMoreSubscribers;
			bool hasMoreSubscriptions;

			for (auto filter_filterID_pair : subscription_subscriptionID_association_table) {
				removeSubscription(sender, (filter_filterID_pair.first), &hasMoreSubscribers, &hasMoreSubscriptions);
			}
		}

		// function matching against dynamic filters of subscriber before forwarding to subscriber:
		virtual bool match(const typename NetworkType::Key & to, typename message::FilterInfo::Ptr filterInfo, typename BaseFilter<EventType, NetworkType>::PayloadPtr event) override {
			// Given the filterinfo (from getPublishPayload), the message and the recipient
			// decide if the recipient needs that message

			// remember result and evaluate again only if needed
			static std::set<typename NetworkType::Key> matching_subscribers; // stores matched subscriptions

			if (!event || current_event_ != event || has_new_subscription_) {
				matching_subscribers.clear();
				has_new_subscription_ = false;
				current_event_ = event;

				for (auto predicate : fulfilled_predicate_vector_) {
					predicate = false;
				}

				for (auto predicate_index : predicate_indexes_) {
					predicate_index->determineMatchingPredicates(*event, fulfilled_predicate_vector_);
				}

				// determine number of fulfilled predicates for each subscription: (hit vector)
				std::map<SubscriptionIdentifierFactory::SubscriptionID, int> hit_vector;

				PredicateIdentifierFactory::PredicateID current_predicate_id = 0;
				for (auto predicate : fulfilled_predicate_vector_) {
					if (predicate) {
						auto hit_subscriptions = predicate_subscription_association_table_[current_predicate_id];
						for (auto hit_subscription : hit_subscriptions) {
							// auto previous_hit_number = hit_vector.at(hit_subscription);
							auto previous_hit_number = 0;
							auto iter_hit_vector = hit_vector.find(hit_subscription);
							if (iter_hit_vector != hit_vector.end()) {
								previous_hit_number = hit_vector.at(hit_subscription);
							}
							hit_vector[hit_subscription] = previous_hit_number + 1;
							M2ETIS_LOG_INFO("GeneralBooleanExpression", "belonging to predicate id " << current_predicate_id << "new hit number " << previous_hit_number + 1);
						}
					}
					++current_predicate_id;
				}

				// determine matching subscription candidates (hit count >= minimum fulfilled predicate count)
				// and test

				for (auto & subscription : subscriptions_) {
					auto iter_hit_vector = hit_vector.find(subscription.first);
					if (iter_hit_vector != hit_vector.end()) {
						// store subscribers so indexes do not have to be evaluated again for the same event
						if (boost::get<0>(subscription.second) <= hit_vector.at(subscription.first)) {
							MatchVisitor<EventType> dynamic_filter_visitor(*event);
							(boost::get<1>(subscription.second))->Accept(dynamic_filter_visitor);

							if (dynamic_filter_visitor.get_result()) {
								auto subscribers_to_subscriptionID = subscription_subscriber_association_table.find(subscription.first)->second;
								matching_subscribers.insert(subscribers_to_subscriptionID.begin(), subscribers_to_subscriptionID.end());
							}
						}
					}
				}
			} // if event has to be evaluated again

			return (matching_subscribers.find(to) == matching_subscribers.end() ? false : true);
		}

		// function matching against own dynamic_filters before delivering to application:
		virtual bool match(typename BaseFilter<EventType, NetworkType>::PayloadPtr event) override {
			return match(self_, typename message::FilterInfo::Ptr(), event);
		}

		virtual void setSelf(const typename NetworkType::Key & self) override {
			self_ = self;
		}

	private:
		// adds the subscription to the data structures
		// called from getSubscribePayload and processSubscribePayload
		void addSubscription(const typename NetworkType::Key & subscriber_key, boost::shared_ptr<FilterExp<EventType>> filter) {
			// test, if subscription is already registered:
			for (auto filter_filterID_pair : subscription_subscriptionID_association_table) {
				if (*(filter_filterID_pair.first) == *filter) {
					subscription_subscriber_association_table[filter_filterID_pair.second].insert(subscriber_key);
					return;
				}
			}

			has_new_subscription_ = true;

			auto subscription_id = subscription_identifier_factory_.createSubscriptionIdentifier();
			subscription_subscriber_association_table[subscription_id] = std::set<typename NetworkType::Key>({subscriber_key});

			GeneralBooleanExpressionsPreProcessVisitor<EventType, NetworkType> preprocess_visitor(&predicate_indexes_, &predicate_subscription_association_table_, subscription_id, &predicate_identifier_factory_);

			filter->Accept(preprocess_visitor);

			// initializing fulfilled_predicate_vector_ with the correct size:
			if (!predicate_subscription_association_table_.empty()) {
				auto maxPredicateID = (--predicate_subscription_association_table_.end())->first;
				fulfilled_predicate_vector_ = std::vector<bool>(maxPredicateID + 1, false);
			} else {
				fulfilled_predicate_vector_ = std::vector<bool>();
			}

			// necessary data structures for subscriptions, containing:
			// subscription id(key), minimum predicate count vector, hit vector, shared pointer to subscription tree
			// filled in getSubscrbie payload, with the exception of hit vector, which is filled in match

			boost::tuple<int, boost::shared_ptr<FilterExp<EventType>>> subscription_data = {0, filter};
			// initialize minimum predicate count vector (Algorithm GetMinPredicates in Bittner)

			GetMinPredicatesVisitor<EventType> get_min_predicates_visitor;

			filter->Accept(get_min_predicates_visitor);

			boost::get<0>(subscription_data) = get_min_predicates_visitor.get_result();

			subscriptions_[subscription_id] = subscription_data;

			subscription_subscriptionID_association_table.push_back(std::make_pair(filter, subscription_id));

			return;
		} // void addSubscription

		// removes the subscription from the data structures
		// called from getUnsubscribePayload and processUnsubscribePayload
		void removeSubscription(const typename NetworkType::Key & subscriber_key, boost::shared_ptr<FilterExp<EventType>> filter, bool * hasMoreSubscribers, bool * hasMoreSubscriptions) {
			*hasMoreSubscriptions = true; // subscriptions to the node!
			*hasMoreSubscribers = true; // further subscribers having registered the predicate to be removed

			// determining subscription id:
			SubscriptionIdentifierFactory::SubscriptionID subscriptionID = 0;
			auto erasable_subscription_iter = subscription_subscriptionID_association_table.end();

			for (auto filter_id_pair_iter = subscription_subscriptionID_association_table.begin(); filter_id_pair_iter != subscription_subscriptionID_association_table.end(); ++filter_id_pair_iter) {
				if (*(filter_id_pair_iter->first) == *filter) {
					subscriptionID = filter_id_pair_iter->second;
					erasable_subscription_iter = filter_id_pair_iter;
				}
			}

			if (erasable_subscription_iter == subscription_subscriptionID_association_table.end()) {
				// subscription not registered
				*hasMoreSubscribers = true;

				// find out if there are subscriptions to this node left:
				checkForSubscriptions(subscriber_key, hasMoreSubscriptions);

				return;
			}

			subscription_subscriber_association_table[subscriptionID].erase(subscriber_key);

			if (subscription_subscriber_association_table[subscriptionID].size() == 0) {
				// no more subscribers for this subscription
				*hasMoreSubscribers = false;
				subscription_subscriber_association_table.erase(subscriptionID); // remove empty set

				subscriptions_.erase(subscriptionID);
				subscription_subscriptionID_association_table.erase(erasable_subscription_iter);

				// using predicate_subscription_association_table_ to determine predicates in removed filter:

				std::set<PredicateIdentifierFactory::PredicateID> erasable_predicates;

				for (auto & predicateID_subscriptionSet_pair : predicate_subscription_association_table_) {
					auto number_of_subscriptions_to_predicate = predicateID_subscriptionSet_pair.second.count(subscriptionID);

					if (number_of_subscriptions_to_predicate == 0) {
						continue;
						// predicate not used in this subscription
					}

					if (number_of_subscriptions_to_predicate == 1) {
						// there are no other subscriptions with this predicate,
						// remove predicate from indexes

						// store predicate ids to remove from predicate_subscription_association_table
						// may not be removed here to avoid invalidating iterators

						erasable_predicates.insert(predicateID_subscriptionSet_pair.first);

						for (auto predicate_index : predicate_indexes_) {
							predicate_index->removePredicate(predicateID_subscriptionSet_pair.first);
						}
					}

					predicateID_subscriptionSet_pair.second.erase(predicateID_subscriptionSet_pair.second.find(subscriptionID));
				}

				for (auto erasable_predicate : erasable_predicates) {
					predicate_subscription_association_table_.erase(erasable_predicate);
				}

				subscription_identifier_factory_.freeID(subscriptionID);
			} // no more subscribers for this subscription

			// find out if there are subscriptions to this node left:

			checkForSubscriptions(subscriber_key, hasMoreSubscriptions);
		} // removeSubscription

		void checkForSubscriptions(const typename NetworkType::Key & subscriber_key, bool * hasMoreSubscriptions) {
			// find out if there are subscriptions to this node left:
			*hasMoreSubscriptions = false;

			for (auto subscription_subscriber_pair : subscription_subscriber_association_table) {
				if (subscription_subscriber_pair.second.count(subscriber_key) > 0) {
					*hasMoreSubscriptions = true;
					break;
				}
			}
		}

		// The key of this node
		typename NetworkType::Key self_;

		typename FilterInfoType::Ptr cast(typename message::FilterInfo::Ptr ptr) const {
			typename FilterInfoType::Ptr ret = boost::dynamic_pointer_cast<FilterInfoType>(ptr);
			if (!ret) {
				M2ETIS_LOG_ERROR("Filter Strategy", "Downcast error of filterInfo");
			}
			return ret;
		}

		// list of predicate indexes: (each of which has to be evaluated for every incoming message)
		std::vector<std::shared_ptr<PredicateIndex<EventType>>> predicate_indexes_;

		// predicate_id corresponds to position in fulfilled_predicate_vector_
		std::vector<bool> fulfilled_predicate_vector_;

		// if predicate is matched, determine subcriptions containing predicate for candidate subscription checking
		// via the following data structure
		// (multi)set because of unsubscriptions
		std::map<PredicateIdentifierFactory::PredicateID, std::multiset<SubscriptionIdentifierFactory::SubscriptionID>> predicate_subscription_association_table_;

		SubscriptionIdentifierFactory subscription_identifier_factory_;
		PredicateIdentifierFactory predicate_identifier_factory_;

		// necessary data structures for subscriptions, containing:
		// subscription id(key), minimum predicate count, shared pointer to subscription tree
		// filled in getSubscrbie payload
		// hit vector is filled in match
		std::map<SubscriptionIdentifierFactory::SubscriptionID, boost::tuple<int, boost::shared_ptr<FilterExp<EventType>>>> subscriptions_;

		std::map<SubscriptionIdentifierFactory::SubscriptionID, std::set<typename NetworkType::Key>> subscription_subscriber_association_table;

		// needed to determine if indexes have to be evaluated again or if previous result can be used:
		bool has_new_subscription_;
		boost::shared_ptr<EventType> current_event_; // not via == operator, so EventType does not have to overload ==

		// mapping filters (which are sent via network) to (local) IDs for deregistrations:
		// maybe introduce hash (unordered_)map to fasten access to filters with identical hashes and their SubscriptionIDs
		std::vector<std::pair<boost::shared_ptr<FilterExp<EventType>>, SubscriptionIdentifierFactory::SubscriptionID>> subscription_subscriptionID_association_table;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANEXPRESSIONSFILTER_H__ */

/**
 * @}
 */
