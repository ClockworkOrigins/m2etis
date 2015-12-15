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

#ifndef __M2ETIS_PUBSUB_FILTER_ATTRIBUTETYPEINFORMATION_H__
#define __M2ETIS_PUBSUB_FILTER_ATTRIBUTETYPEINFORMATION_H__

#include <memory>

#include "m2etis/util/Exceptions.h"

#include "m2etis/pubsub/filter/decisiontree/DecisionTreeNode.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/PredicateIdentifierFactory.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/SubscriptionIdentifierFactory.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/PredicateIndex.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/GreaterThanPredicateIndex.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/LessThanPredicateIndex.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/EqualsPredicateIndex.h"
#include "m2etis/pubsub/filter/generalbooleanalgebra/NotEqualsPredicateIndex.h"
#include "m2etis/pubsub/filter/decisiontree/AttributeDecisionTreeNode.h"

#include "boost/make_shared.hpp"

/**
 * @class AttributeTypeInformation
 *
 * This class is intended to simplify the implementation of filter strategies
 *
 * Filters of type FilterExp may contain template parameters like the attribute type of an attribute to be constrained
 *
 * The visitors which operate on FilterExp are related via inheritance and may contain virtual functions.
 * Those virtual functions may not contain template parameters.
 * To avoid writing those virtual functions for each attribute types occuring in filters,
 *  the visitors may call functions from the class AttributeTypeInformation
 */

namespace m2etis {
namespace pubsub {
namespace filter {

	typedef int AttributeName;

	template<typename EventType, typename AttributeType> class AttributeFilter;
	template<typename EventType, typename AttributeType> class EqualsAttributeFilter;
	template<typename EventType, typename AttributeType> class NotEqualsAttributeFilter;
	template<typename EventType, typename AttributeType> class GreaterThanAttributeFilter;
	template<typename EventType, typename AttributeType> class LessThanAttributeFilter;

	template<typename EventType, typename NetworkType> class DecisionTreePreProcessVisitor;
	template<typename EventType, typename NetworkType> class DecisionTreeFilter;
	template<typename EventType, typename NetworkType> class DecisionTreeWalkerVisitor;

	template<typename EventType, typename NetworkType> class GeneralBooleanExpressionsPreProcessVisitor;
	template<typename EventType, typename NetworkType> class GeneralBooleanExpressionsFilter;

	template<typename EventType> class PredicateIndex;

	// class to gain access to attribute type of filter:
	// helpful to avoid writing duplicate code for different attribute type, e.g. in DecisionTreePreProcessVisitor
	// or GeneralBooleanExpressionsPreProcessVisitor

	template<typename EventType, typename NetworkType>
	class AttributeTypeInformation {
	public:
		friend class DecisionTreePreProcessVisitor<EventType, NetworkType>;
		friend class DecisionTreeWalkerVisitor<EventType, NetworkType>;

		template<typename AttributeType> void operator()(const AttributeFilter<EventType, AttributeType> * equals_predicate, DecisionTreeFilter<EventType, NetworkType> *) {
			// DecisionTreeFilter parameter for overloading purposes

			if (!*decision_tree_node_ptr_) {
				// node has to be added to subscription tree
				*decision_tree_node_ptr_ = boost::make_shared<AttributeDecisionTreeNode<EventType, NetworkType, AttributeType> >(AttributeDecisionTreeNode<EventType, NetworkType, AttributeType>(equals_predicate->get_attribute_id()));
			}

			if ((equals_predicate->get_constants()).empty()) {
				// follow dont care edge
				auto & dont_care_subnode_ptr_ = dynamic_cast<AttributeDecisionTreeNode<EventType, NetworkType, AttributeType>*>(decision_tree_node_ptr_->get())->dont_care_subnode_ptr_;

				decision_tree_node_ptr_ = &(dont_care_subnode_ptr_);
			} else {
				// look for edge with value of attribute type
				auto & result_subnodes = dynamic_cast<AttributeDecisionTreeNode<EventType, NetworkType, AttributeType>*>(decision_tree_node_ptr_->get())->result_subnodes_;

				auto result_subnode_iterator = result_subnodes.find(equals_predicate->get_constants()[0]);

				if (result_subnode_iterator != result_subnodes.end()) {
					// v has an edge e with the result -> follow the edge
					decision_tree_node_ptr_ = &(result_subnode_iterator->second);
				} else {
					// create new edge and follow (shared pointer is null, initialized in this function when visiting the next node)
					result_subnodes[equals_predicate->get_constants()[0]] = boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> >();
					// TODO: (Roland) check if really null
					decision_tree_node_ptr_ = &(result_subnodes[equals_predicate->get_constants()[0]]);
				}
			}
		}

		template<typename AttributeType> void operator()(const AttributeFilter<EventType, AttributeType> * equals_predicate, DecisionTreeFilter<EventType, NetworkType> *, int) {
			// walks through the decision tree
			// DecisionTreeFilter and int parameter  for overloading purposes

			// look for edge with value of attribute in filte
			if ((equals_predicate->get_constants()).empty()) {
				// follow dont care edge
				auto & dont_care_subnode_ptr_ = dynamic_cast<AttributeDecisionTreeNode<EventType, NetworkType, AttributeType>*>(decision_tree_node_ptr_->get())->dont_care_subnode_ptr_;

				decision_tree_node_ptr_ = &(dont_care_subnode_ptr_);
			} else {
				// look for edge with value of attribute type
				auto & result_subnodes = dynamic_cast<AttributeDecisionTreeNode<EventType, NetworkType, AttributeType>*>(decision_tree_node_ptr_->get())->result_subnodes_;

				auto result_subnode_iterator = result_subnodes.find(equals_predicate->get_constants()[0]);

				if (result_subnode_iterator != result_subnodes.end()) {
					// v has an edge e with the result -> follow the edge

					decision_tree_node_ptr_ = &(result_subnode_iterator->second);
				} else {
					// no edge contained <= filter not registered
					// set decision_tree_node_ptr to null to signal
					// not registered filter
					decision_tree_node_ptr_ = nullptr;
				}
			}
		} // operator() for walking decision trees with given filter

		friend class GeneralBooleanExpressionsPreProcessVisitor<EventType, NetworkType>;

		template<typename AttributeType> void operator()(const AttributeFilter<EventType, AttributeType> *, GeneralBooleanExpressionsFilter<EventType, NetworkType> *) {
			M2ETIS_THROW_API("AttributeTypeInformation", "function called for AttributeFilter. Should be called for special kind.");
		}

		template<typename AttributeType> void operator()(const EqualsAttributeFilter<EventType, AttributeType>* equals_predicate, GeneralBooleanExpressionsFilter<EventType, NetworkType> *) { // overloaded for general boolean expressions
			// check if predicate index for combination of attribute name and operator already exists:
			bool has_predicate = false;

			for (auto predicate_index : (*predicate_indexes_)) {
				if ((predicate_index)->get_operator() == EQUALS && (predicate_index)->get_attribute_id() == equals_predicate->get_attribute_id()) {
					// add value to existing predicate index
					has_predicate = true;
					auto equals_predicate_index = std::dynamic_pointer_cast<EqualsPredicateIndex<EventType, AttributeType> >(predicate_index);

					auto new_predicate_id = equals_predicate_index->addPredicate(equals_predicate);
					// store predicate identifier , that is returned, in predicate subscription association table
					// maybe more than once

					if (predicate_subscription_association_table_->find(new_predicate_id) == predicate_subscription_association_table_->end()) {
						(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
					} else {
						(*predicate_subscription_association_table_)[new_predicate_id].insert(current_subscription_id_);
					}
					break;
				}
			}
			if (!has_predicate) {
				// create new predicate index and add value;
				auto equals_predicate_index = std::make_shared<EqualsPredicateIndex<EventType, AttributeType> >(equals_predicate->get_attribute_id(), predicate_identifier_factory_);

				predicate_indexes_->push_back(equals_predicate_index);

				auto new_predicate_id = equals_predicate_index->addPredicate(equals_predicate);

				(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
			}
		} // Equals for GeneralBooleanExpressions

		template<typename AttributeType> void operator()(const NotEqualsAttributeFilter<EventType, AttributeType> * notequals_predicate, GeneralBooleanExpressionsFilter<EventType, NetworkType> *) { // overloaded for general boolean expressions
			// check if predicate index for combination of attribute name and operator already exists:
			bool has_predicate = false;

			for (auto predicate_index : (*predicate_indexes_)) {
				if ((predicate_index)->get_operator() == NOTEQUALS && (predicate_index)->get_attribute_id() == notequals_predicate->get_attribute_id()) {
					// add value to existing predicate index
					has_predicate = true;
					auto notequals_predicate_index = std::dynamic_pointer_cast<NotEqualsPredicateIndex<EventType, AttributeType> >(predicate_index);

					auto new_predicate_id = notequals_predicate_index->addPredicate(notequals_predicate);
					// store predicate identifier , that is returned, in predicate subscription association table
					// maybe more than once

					if (predicate_subscription_association_table_->find(new_predicate_id) == predicate_subscription_association_table_->end()) {
						(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
					} else {
						(*predicate_subscription_association_table_)[new_predicate_id].insert(current_subscription_id_);
					}
					break;
				}
			}
			if (!has_predicate) {
				// create new predicate index and add value;
				auto notequals_predicate_index = std::make_shared<NotEqualsPredicateIndex<EventType, AttributeType> >(notequals_predicate->get_attribute_id(), predicate_identifier_factory_);

				predicate_indexes_->push_back(notequals_predicate_index);

				auto new_predicate_id = notequals_predicate_index->addPredicate(notequals_predicate);

				(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
			}
		} // NotEquals for GeneralBooleanExpressions

		template<typename AttributeType> void operator()(const GreaterThanAttributeFilter<EventType, AttributeType> * greater_than_predicate, GeneralBooleanExpressionsFilter<EventType, NetworkType> *) { // overloaded for general boolean expressions
			// check if predicate index for combination of attribute name and operator already exists:
			bool has_predicate = false;
			for (auto predicate_index : (*predicate_indexes_)) {
				if ((predicate_index)->get_operator() == GREATERTHAN && (predicate_index)->get_attribute_id() == greater_than_predicate->get_attribute_id()) {
					// predicate index already exists

					// add value to existing predicate index
					has_predicate = true;
					auto greaterthan_predicate_index = std::dynamic_pointer_cast<GreaterThanPredicateIndex<EventType, AttributeType> >(predicate_index);

					auto new_predicate_id = greaterthan_predicate_index->addPredicate(greater_than_predicate);
					// store predicate identifier , that is returned, in predicate subscription association table
					// maybe more than once

					if (predicate_subscription_association_table_->find(new_predicate_id) == predicate_subscription_association_table_->end()) {
						(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
					} else {
						(*predicate_subscription_association_table_)[new_predicate_id].insert(current_subscription_id_);
					}
					break;
				}
			}
			if (!has_predicate) {
				// create new predicate index and add value;
				auto greater_than_predicate_index = std::make_shared<GreaterThanPredicateIndex<EventType, AttributeType> >(greater_than_predicate->get_attribute_id(), predicate_identifier_factory_);

				predicate_indexes_->push_back(greater_than_predicate_index);

				auto new_predicate_id = greater_than_predicate_index->addPredicate(greater_than_predicate);

				(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
			}
		} // GreaterThan

		template<typename AttributeType> void operator()(const LessThanAttributeFilter<EventType, AttributeType> * less_than_predicate, GeneralBooleanExpressionsFilter<EventType, NetworkType> *) { // overloaded for general boolean expressions
			// check if predicate index for combination of attribute name and operator already exists:
			bool has_predicate = false;
			for (auto predicate_index : (*predicate_indexes_)) {
				if ((predicate_index)->get_operator() == LESSTHAN && (predicate_index)->get_attribute_id() == less_than_predicate->get_attribute_id()) {
					// add value to existing predicate index
					has_predicate = true;
					auto lessthan_predicate_index = std::dynamic_pointer_cast<LessThanPredicateIndex<EventType, AttributeType> >(predicate_index);

					auto new_predicate_id = lessthan_predicate_index->addPredicate(less_than_predicate);
					// store predicate identifier , that is returned, in predicate subscription association table
					// maybe more than once

					if (predicate_subscription_association_table_->find(new_predicate_id) == predicate_subscription_association_table_->end()) {
						(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
					} else {
						(*predicate_subscription_association_table_)[new_predicate_id].insert(current_subscription_id_);
					}
					break;
				}
			}
			if (!has_predicate) {
				// create new predicate index and add value;
				auto less_than_predicate_index = std::make_shared<LessThanPredicateIndex<EventType, AttributeType> >(less_than_predicate->get_attribute_id(), predicate_identifier_factory_);

				predicate_indexes_->push_back(less_than_predicate_index);

				auto new_predicate_id = less_than_predicate_index->addPredicate(less_than_predicate);

				(*predicate_subscription_association_table_)[new_predicate_id] = std::multiset<SubscriptionIdentifierFactory::SubscriptionID>{current_subscription_id_};
			}
		} // LessThan

	private:
		// the following attributes are usually set by the boolean expression visitor
		// ******** attributes for decision tree filter ********************
		boost::shared_ptr<DecisionTreeNode<EventType, NetworkType> > * decision_tree_node_ptr_;

		// ******** attributes for general boolean expressions filter ******
		std::vector<std::shared_ptr<PredicateIndex<EventType> > > * predicate_indexes_;
		std::map<PredicateIdentifierFactory::PredicateID, std::multiset<SubscriptionIdentifierFactory::SubscriptionID> > * predicate_subscription_association_table_;

		PredicateIdentifierFactory * predicate_identifier_factory_;

		SubscriptionIdentifierFactory::SubscriptionID current_subscription_id_;
	};
	// AttributeTypeInforamtion

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_ATTRIBUTETYPEINFORMATION_H__ */

/**
 *  @}
 */
