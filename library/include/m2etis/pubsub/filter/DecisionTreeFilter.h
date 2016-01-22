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
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_FILTER_DECISIONTREEFILTER_H__
#define __M2ETIS_PUBSUB_FILTER_DECISIONTREEFILTER_H__

#include "m2etis/pubsub/filter/FilterPredicate.h"
#include "m2etis/pubsub/filter/filtervisitors/MatchVisitor.h"
#include "m2etis/message/info/filter/DecisionTreeFilterInfo.h"
#include "m2etis/pubsub/filter/filtervisitors/DecisionTreePreProcessVisitor.h"
#include "m2etis/pubsub/filter/filtervisitors/DecisionTreeWalkerVisitor.h"

#include "m2etis/pubsub/filter/decisiontree/DecisionTreeNode.h"
#include "m2etis/pubsub/filter/decisiontree/DecisionTreeLeaf.h"
#include "m2etis/pubsub/filter/decisiontree/AttributeDecisionTreeNode.h"

#include "boost/shared_ptr.hpp"

/**
 * @class DecisionTreeFilter
 *
 * This class filters events by matching with Aguilera´s algorithm
 *
 * prerequisites for aguilera filter strategy:
 * - prerequisite conjunctive filters of equality predicates ordered by attribute id
 * - all filters must contain the same attributes in the same order
 * - a dont care edge may be represented by an empty constant vector in the EqualsAttributeFilter
 *
 * example for the creation of a filter which fulfills the prerequisites mentioned above:
 * 		std::vector<std::string> region_= {std::string("Erlangen") };
		std::vector<int> x_ = {};
		std::vector<int>  y_ = {20};

		auto subscribe_filter = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> > (
				Equals<Position, int>(POSITION_X, {x_}) && Equals<Position, std::string>(POSITION_REGION, {region_}) &&
      		    Equals<Position, int>(POSITION_Y, {y_})
      				);
 *
 *
 */

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class DecisionTreeFilter: public BaseFilter<EventType, NetworkType> {
	public:
		typedef message::DecisionTreeFilterInfo<EventType, NetworkType> FilterInfoType;
		enum {
			size = 0
		};

		DecisionTreeFilter() : decision_tree_(), has_new_subscription_(false), current_event_() {
		}

		virtual ~DecisionTreeFilter() {
		}

		virtual void getSubscribePayload(boost::shared_ptr<FilterExp<EventType> > filter, bool is_periodic_resubscribe, typename message::FilterInfo::Ptr filterInfo) override {
			// implementation of Aguilera´s pre_process algorithm to create subscription decision tree:
			DecisionTreePreProcessVisitor<EventType, NetworkType> decision_tree_preprocess_visitor(&decision_tree_);

			filter->Accept(decision_tree_preprocess_visitor); // add inner nodes to decision tree

			// add subscriber to decision tree
			if (*(decision_tree_preprocess_visitor.get_current_decision_tree_node_ptr())) {
				// subscription node already in tree
				(*decision_tree_preprocess_visitor.get_current_decision_tree_node_ptr())->addSubscriber(self_);
			} else {
				// create node for subscription:
				*(decision_tree_preprocess_visitor.get_current_decision_tree_node_ptr()) = boost::make_shared<DecisionTreeLeaf<EventType, NetworkType> >(self_);
			}

			has_new_subscription_ = true;
			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			fInfo->dynamic_filter_ = decision_tree_;

			fInfo->isUnsubscribe_ = false;
		}

		virtual void getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo) override {
			// when unsubscribing a filter only the sender key in the leaf node is removed for efficiency reasons
			// the actual nodes are not removed and maybe do not lead to any subscribers any more
			decision_tree_ = boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> >();
			typename FilterInfoType::Ptr fInfo = cast(filterInfo);

			fInfo->dynamic_filter_ = boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> >();
		}

		typename BaseFilter<EventType, NetworkType>::FilterUnsubscribeInformation getUnsubscribePayload(typename message::FilterInfo::Ptr filterInfo, boost::shared_ptr<FilterExp<EventType> > filter) {
			DecisionTreeWalkerVisitor<EventType, NetworkType> decision_tree_walker_visitor(&decision_tree_);

			filter->Accept(decision_tree_walker_visitor);

			if (*(decision_tree_walker_visitor.get_current_decision_tree_node_ptr())) {
				// filter is registered
				(*(decision_tree_walker_visitor.get_current_decision_tree_node_ptr()))->removeSubscriber(self_);
			}

			if (!(decision_tree_->hasSubscription(self_))) {
				// if last filter has been deregistered, unsubscribe from the tree
				typename FilterInfoType::Ptr fInfo = cast(filterInfo);
				fInfo->dynamic_filter_ = boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> >();

				return BaseFilter<EventType, NetworkType>::UNSUBSCRIBEFROMTREE;
			}

			// find out, if a subscriber has reigstered the same filter:
			if (*(decision_tree_walker_visitor.get_current_decision_tree_node_ptr())) {
				// filter is registered
				if ((*(decision_tree_walker_visitor.get_current_decision_tree_node_ptr()))->hasSubscriber()) {
					// filter has been deregsitered, but a subscriber of this node has registered the same filter
					return BaseFilter<EventType, NetworkType>::CANCELUNSUBSCRIBE;
				}
			}

			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			fInfo->unsubscribe_filter_ = filter;
			fInfo->isUnsubscribe_ = true;

			return BaseFilter<EventType, NetworkType>::FORWARDUNSUBSCRIBE;
		}

		virtual std::string getPublishPayload(const typename BaseFilter<EventType, NetworkType>::PayloadPtr message_text) const override {
			return ""; // prefilter or preprocess the message and give other nodes a hint
		}

		virtual std::string processSubscribePayload(const typename NetworkType::Key & sender, typename message::FilterInfo::Ptr filterInfo) override {
			typename FilterInfoType::Ptr fInfo = cast(filterInfo);
			if (fInfo->isUnsubscribe_) {
				// currently only one filter can be contained:
				DecisionTreeWalkerVisitor<EventType, NetworkType> decision_tree_walker_visitor(&decision_tree_);

				fInfo->unsubscribe_filter_->Accept(decision_tree_walker_visitor);

				if (*(decision_tree_walker_visitor.get_current_decision_tree_node_ptr())) {
					// filter is registered
					(*(decision_tree_walker_visitor.get_current_decision_tree_node_ptr()))->removeSubscriber(sender);
				}

				return "";
			}

			// decision trees are sent in filterinfo. merge into own decision tree:

			if (!decision_tree_) {
				decision_tree_ = fInfo->dynamic_filter_;
			} else {
				decision_tree_->merge(fInfo->dynamic_filter_, sender);
			}

			has_new_subscription_ = true;
			return "";
		} // processSubscribePayload

		// filterinfo from sender (maybe a predicate)
		virtual void processUnsubscribePayload(const typename NetworkType::Key & sender, typename message::FilterInfo::Ptr filterInfo) override {
			decision_tree_->removeSubscriber(sender);
		}

		// called by routing strategy:
		virtual void processRoutingStrategyUnsubscribeNotification(const typename NetworkType::Key & sender) override {
			decision_tree_->removeSubscriber(sender);
		}

		// function matching against dynamic filters of subscriber before forwarding to subscriber:
		bool match(const typename NetworkType::Key & to, typename message::FilterInfo::Ptr filterInfo, typename BaseFilter<EventType, NetworkType>::PayloadPtr event) {
			static std::set<typename NetworkType::Key> matching_subscribers; // stores matched subscriptions

			if (!event || current_event_ != event || has_new_subscription_) {
				// subscription not already determined
				matching_subscribers.clear();
				has_new_subscription_ = false;
				current_event_ = event;
				decision_tree_->visit(*event, matching_subscribers);
			}

			return (matching_subscribers.find(to) == matching_subscribers.end() ? false : true);
		}

		// function matching against own dynamic_filters before delivering to application:
		bool match(typename BaseFilter<EventType, NetworkType>::PayloadPtr event) {
			return match(self_, typename message::FilterInfo::Ptr(), event);
		}

		void setSelf(const typename NetworkType::Key & self) {
			self_ = self;
		}

	private:
		// The key of this node
		typename NetworkType::Key self_;

		typename FilterInfoType::Ptr cast(typename message::FilterInfo::Ptr ptr) const {
			typename FilterInfoType::Ptr ret = boost::dynamic_pointer_cast<FilterInfoType>(ptr);
			if (!ret) {
				M2ETIS_LOG_ERROR("Filter Strategy", "Downcast error of filterInfo");
			}
			return ret;
		}

		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > decision_tree_;

		// needed to determine if decision tree has to be evaluated again or if previous result can be used:
		bool has_new_subscription_;
		boost::shared_ptr<EventType> current_event_; // not via == operator, so EventType does not have to overload ==
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_DECISIONTREEFILTER_H__ */

/**
 *  @}
 */
