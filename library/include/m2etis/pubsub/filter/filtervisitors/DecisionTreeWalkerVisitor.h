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

#ifndef __M2ETIS_PUBSUB_FILTER_DECISIONTREEWALKERVISITOR_H__
#define __M2ETIS_PUBSUB_FILTER_DECISIONTREEWALKERVISITOR_H__

#include "m2etis/pubsub/filter/AttributeTypeInformation.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class DecisionTreeWalkerVisitor : public FilterVisitor<EventType> {
	public:
		DecisionTreeWalkerVisitor(boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * v) : current_decision_tree_node_ptr_(v) {
		}

		virtual ~DecisionTreeWalkerVisitor() {
		}

		virtual void Visit(const FilterExp<EventType> *) override {
			M2ETIS_THROW_API("DecisionTreeRemoveSubscriberVisitor", "FilterExp not allowed in decision tree filter strategy.");
		}

		virtual void Visit(const AndExp<EventType> *) override {
		}

		virtual void Visit(const OrExp<EventType> * orexp) override {
			M2ETIS_THROW_API("DecisionTreeRemoveSubscriberVisitor", "OrExp not allowed in decision tree filter strategy.");
		}

		virtual void Visit(const Predicate<EventType> * equals_predicate) override {
			if (current_decision_tree_node_ptr_) {
				// if pointer is zero the specified filter for walking the tree is not registered
				attribute_type_information.decision_tree_node_ptr_ = current_decision_tree_node_ptr_;

				equals_predicate->getAttributeType(*this);

				current_decision_tree_node_ptr_ = attribute_type_information.decision_tree_node_ptr_;
			}
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, int> * filter) {
			auto decision_tree_ptr = new DecisionTreeFilter<EventType, NetworkType>();
			attribute_type_information(filter, decision_tree_ptr, 0);
			delete decision_tree_ptr;
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, std::string> * filter) {
			auto decision_tree_ptr = new DecisionTreeFilter<EventType, NetworkType>();
			attribute_type_information(filter, decision_tree_ptr, 0);
			delete decision_tree_ptr;
		}

		using FilterVisitor<EventType>::getAttributeType;

		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * get_current_decision_tree_node_ptr() {
			return current_decision_tree_node_ptr_;
		}

	private:
		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * current_decision_tree_node_ptr_;

		AttributeTypeInformation<EventType, NetworkType> attribute_type_information;
	};
	// class DecisionTreeWalkerVisitor

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_DECISIONTREEWALKERVISITOR_H__ */

/**
 *  @}
 */
