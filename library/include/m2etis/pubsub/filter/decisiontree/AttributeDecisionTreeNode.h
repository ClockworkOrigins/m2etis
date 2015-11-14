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

#ifndef __M2ETIS_PUBSUB_FILTER_DECISIONTREE_ATTRIBUTEDECISIONTREENODE_H__
#define __M2ETIS_PUBSUB_FILTER_DECISIONTREE_ATTRIBUTEDECISIONTREENODE_H__

#include <set>
#include <string>

#include "m2etis/pubsub/filter/AttributeAccessor.h"

#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace pubsub {

	M2ETIS_API extern std::map<filter::AttributeName, std::shared_ptr<filter::AttributeAccessor_Basic> > attributes_accessor_map;

namespace filter {

	template<typename EventType, typename NetworkType> class DecisionTreePreProcessVisitor;
	template<typename EventType, typename NetworkType> class AttributeTypeInformation;

	typedef int AttributeName; // TODO: (Roland) already defined in AttributeTypeInformation

	template<typename EventType, typename NetworkType, typename AttributeType>
	class AttributeDecisionTreeNode : public DecisionTreeNode<EventType, NetworkType> {
	public:
		explicit AttributeDecisionTreeNode(AttributeName attribute_id) : attribute_id_(attribute_id) {
		}
		AttributeDecisionTreeNode() : attribute_id_() {
		}

		// equivalent to match algorithm in Aguilera
		virtual void visit(const EventType & event, std::set<typename NetworkType::Key> & matching_subscribers) override {
			// perform test prescribed by v on event (i.e. determine attribute value)
			auto attribute_value_iterator = attributes_accessor_map.find(attribute_id_);

			if (attribute_value_iterator == attributes_accessor_map.end()) {
				M2ETIS_THROW_API("AttributeDecisionTreeNode function visit", "Attribute not found in attribute map.");
			}

			// look for edge with value of attribute

			auto result_subnode_iterator = result_subnodes_.find((dynamic_cast<AttributeAccessor<EventType, AttributeType> *>(attribute_value_iterator->second.get()))->getAttributeValue(event));

			if (result_subnode_iterator != result_subnodes_.end()) {
				// v has an edge e with the result of test (i.e. attribute value)
				result_subnode_iterator->second->visit(event, matching_subscribers);
			}

			if (dont_care_subnode_ptr_) {
				dont_care_subnode_ptr_->visit(event, matching_subscribers);
			}
		}

		// merges with another subscription tree received by subscriptions from subscribes
		virtual void merge(boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > & other_tree, typename NetworkType::Key subscriber) override {
			auto other_tree_node = boost::dynamic_pointer_cast<AttributeDecisionTreeNode<EventType, NetworkType, AttributeType> >(other_tree);

			if (other_tree_node->dont_care_subnode_ptr_) {
				if (dont_care_subnode_ptr_) {
					// dont care edge already exists in both trees about to be merged
					dont_care_subnode_ptr_->merge(other_tree_node->dont_care_subnode_ptr_, subscriber);
				} else {
					dont_care_subnode_ptr_ = other_tree_node->dont_care_subnode_ptr_;
					dont_care_subnode_ptr_->changeSubscriber(subscriber);
					// newly inserted nodes belong to the subscriber that sent the filter
				}
			}

			for (auto other_tree_node_subnodes : other_tree_node->result_subnodes_) {
				if (result_subnodes_.find(other_tree_node_subnodes.first) != result_subnodes_.end()) {
					// edge existing in both trees

					result_subnodes_[other_tree_node_subnodes.first]->merge(other_tree_node_subnodes.second, subscriber);
				} else {
					result_subnodes_[other_tree_node_subnodes.first] = other_tree_node_subnodes.second;
					result_subnodes_[other_tree_node_subnodes.first]->changeSubscriber(subscriber);
				}
			}
		} // merge

		virtual void changeSubscriber(typename NetworkType::Key subscriber) override {
			for (auto subnodes : result_subnodes_) {
				subnodes.second->changeSubscriber(subscriber);
			}

			if (dont_care_subnode_ptr_) {
				dont_care_subnode_ptr_->changeSubscriber(subscriber);
			}
		} // changeSubscriber

		// Removes the node from the tree; returns number of children left
		virtual void removeSubscriber(const typename NetworkType::Key & subscriber) override {
			for (auto subnodes : result_subnodes_) {
				subnodes.second->removeSubscriber(subscriber);
			}

			if (dont_care_subnode_ptr_) {
				dont_care_subnode_ptr_->removeSubscriber(subscriber);
			}
		}

		// Testing whether there are subscriptions to the subscriber
		virtual bool hasSubscription(typename NetworkType::Key subscriber) override {
			bool hasSubscriptionResult = false;
			for (auto subnodes : result_subnodes_) {
				hasSubscriptionResult = hasSubscriptionResult
						|| subnodes.second->hasSubscription(subscriber);
			}

			if (dont_care_subnode_ptr_) {
				hasSubscriptionResult = hasSubscriptionResult
						|| dont_care_subnode_ptr_->hasSubscription(subscriber);
			}

			return hasSubscriptionResult;
		}

		virtual operator std::string() const {
			std::string temp = "attribute node with attribute_id: " + std::to_string(attribute_id_) + "subnodes:";
			for (auto subnodes : result_subnodes_) {
				temp += "edge: with value (not computable) next node:" + std::string(*(subnodes.second));
			}
			return temp;
		}

		virtual ~AttributeDecisionTreeNode() {
		}

		friend class DecisionTreePreProcessVisitor<EventType, NetworkType>;
		friend class AttributeTypeInformation<EventType, NetworkType>;

	private:
		std::map<AttributeType, boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > > result_subnodes_; // maps each value to the corresponding subnode in the decision tree.

		/* -edge (does not fit into result_subnode_map), for subscriptions that don´t care about the
		 * value of this attribute
		 */
		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > dont_care_subnode_ptr_;

		AttributeName attribute_id_;

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & boost::serialization::base_object<DecisionTreeNode<EventType, NetworkType> >(*this);
			ar & result_subnodes_;
			ar & dont_care_subnode_ptr_;
			ar & attribute_id_;
		}
	};
	// AttributeDecisionTreeNode

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_DECISIONTREE_ATTRIBUTEDECISIONTREENODE_H__ */

/**
 *  @}
 */
