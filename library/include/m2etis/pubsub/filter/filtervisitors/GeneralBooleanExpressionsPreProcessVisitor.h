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

#ifndef __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANEXPRESSIONSPREPROCESSVISITOR_H__
#define __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANEXPRESSIONSPREPROCESSVISITOR_H__

#include <set>

#include "m2etis/pubsub/filter/FilterPredicate.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template<typename EventType, typename NetworkType>
	class GeneralBooleanExpressionsPreProcessVisitor : public FilterVisitor<EventType> {
	public:
		GeneralBooleanExpressionsPreProcessVisitor(std::vector<std::shared_ptr<PredicateIndex<EventType> > > * predicate_indexes, std::map<PredicateIdentifierFactory::PredicateID, std::multiset<SubscriptionIdentifierFactory::SubscriptionID> > * predicate_subscription_association_table, SubscriptionIdentifierFactory::SubscriptionID current_subscription_id, PredicateIdentifierFactory * predicate_identifier_factory) : predicate_indexes_(predicate_indexes), predicate_subscription_association_table_(predicate_subscription_association_table), current_subscription_id_(current_subscription_id), predicate_identifier_factory_(predicate_identifier_factory) {
		}

		virtual ~GeneralBooleanExpressionsPreProcessVisitor() {
		}

		virtual void Visit(const FilterExp<EventType> *) override {
			M2ETIS_THROW_API("GeneralBooleanExpressionsPreProcessVisitor", "FilterExp not allowed in general boolean expressions filter strategy.");
		}

		virtual void Visit(const AndExp<EventType> *) override {
			// not needed for predicate indexing
		}

		virtual void Visit(const OrExp<EventType> * orexp) override {
			// not needed for predicate indexing
		}

		virtual void Visit(const Predicate<EventType> * current_predicate) override {
			attribute_type_information.predicate_indexes_ = predicate_indexes_;
			attribute_type_information.predicate_subscription_association_table_ = predicate_subscription_association_table_;
			attribute_type_information.predicate_identifier_factory_ = predicate_identifier_factory_;
			attribute_type_information.current_subscription_id_ = current_subscription_id_;

			current_predicate->getAttributeType(*this);
		}

		virtual void getAttributeType(const GreaterThanAttributeFilter<EventType, std::string> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		}

		virtual void getAttributeType(const GreaterThanAttributeFilter<EventType, int> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		}

		virtual void getAttributeType(const LessThanAttributeFilter<EventType, std::string> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		}

		virtual void getAttributeType(const LessThanAttributeFilter<EventType, int> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		}

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, std::string> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		} // getAttribute Type for EqualsAttributeFilter and string

		virtual void getAttributeType(const EqualsAttributeFilter<EventType, int> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		} // getAttributeType for EqualsAttributeFilter and int

		virtual void getAttributeType(const NotEqualsAttributeFilter<EventType, std::string> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		}

		virtual void getAttributeType(const NotEqualsAttributeFilter<EventType, int> * filter) override {
			auto general_boolean_expressions_ptr = new GeneralBooleanExpressionsFilter<EventType, NetworkType>();
			attribute_type_information(filter, general_boolean_expressions_ptr);
			delete general_boolean_expressions_ptr;
		}

		using FilterVisitor<EventType>::getAttributeType;

	private:
		// reference to predicate_indexes in filter class:
		std::vector<std::shared_ptr<PredicateIndex<EventType> > > * predicate_indexes_;

		std::map<PredicateIdentifierFactory::PredicateID, std::multiset<SubscriptionIdentifierFactory::SubscriptionID> > * predicate_subscription_association_table_;
		SubscriptionIdentifierFactory::SubscriptionID current_subscription_id_;
		PredicateIdentifierFactory * predicate_identifier_factory_;

		AttributeTypeInformation<EventType, NetworkType> attribute_type_information;
	};
	// class GeneralBooleanExpressionsPreProcessVisitor

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANEXPRESSIONSPREPROCESSVISITOR_H__ */

/**
 *  @}
 */
