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

#ifndef __M2ETIS_PUBSUB_FILTER_DECISIONTREEPREPROCESSVISITOR_H__
#define __M2ETIS_PUBSUB_FILTER_DECISIONTREEPREPROCESSVISITOR_H__

#include "m2etis/pubsub/filter/AttributeTypeInformation.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

/**
 * @class DecisionTreePreProcesssVisitor
 *
 * This class is used to build the decision tree from scratch
 */

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class DecisionTreePreProcessVisitor : public FilterVisitor<EventType> {
	public:
		DecisionTreePreProcessVisitor(boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * v) : current_decision_tree_node_ptr_(v) {
		}

		virtual ~DecisionTreePreProcessVisitor() {
		}

		// not allowed for OrExp
		virtual void Visit(const FilterExp<EventType> *) override {
			M2ETIS_THROW_API("DecisionTreeProcessVisitor", "FilterExp not allowed in decision tree filter strategy.");
		}

		virtual void Visit(const AndExp<EventType> *) override {
		}

		virtual void Visit(const OrExp<EventType> * orexp) override {
			M2ETIS_THROW_API("DecisionTreeProcessVisitor", "OrExp not allowed in decision tree filter strategy.");
		}

		virtual void Visit(const Predicate<EventType> * equals_predicate) override {
			// order of preprocess algorithm differs from description in aguilera´s paper
			// because in follow the attribute type of the next node is not known
			attribute_type_information.decision_tree_node_ptr_ = current_decision_tree_node_ptr_;

			equals_predicate->getAttributeType(*this);

			current_decision_tree_node_ptr_ = attribute_type_information.decision_tree_node_ptr_;
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, std::string> * filter) {
			auto decision_tree_ptr = new DecisionTreeFilter<EventType, NetworkType>();
			attribute_type_information(filter, decision_tree_ptr);
			delete decision_tree_ptr;
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, int> * filter) {
			auto decision_tree_ptr = new DecisionTreeFilter<EventType, NetworkType>();
			attribute_type_information(filter, decision_tree_ptr);
			delete decision_tree_ptr;
		}

		using FilterVisitor<EventType>::getAttributeType;

		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * get_current_decision_tree_node_ptr() {
			return current_decision_tree_node_ptr_;
		}

	private:
		// this is the pointer to the shared_ptr in the map of the parent node
		// the attribute type is not known until accessing the next node, respectively
		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * current_decision_tree_node_ptr_;

		AttributeTypeInformation<EventType, NetworkType> attribute_type_information;
	};
	// class DecisionTreePreProcessVisitor

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_DECISIONTREEPREPROCESSVISITOR_H__ */

/**
 *  @}
 */
