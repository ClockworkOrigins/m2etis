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

#ifndef __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_GREATERTHANPREDICATEINDEX_H__
#define __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_GREATERTHANPREDICATEINDEX_H__

#include "m2etis/pubsub/filter/filterexpressions/GreaterThanAttributeFilter.h"

namespace m2etis {
namespace pubsub {
namespace filter {

	template <typename EventType, typename AttributeType>
	class GreaterThanPredicateIndex : public PredicateIndex<EventType>  {
	public:
		GreaterThanPredicateIndex(AttributeName attribute_id, PredicateIdentifierFactory * predicate_identifier_factory) : PredicateIndex<EventType>(attribute_id, GREATERTHAN, predicate_identifier_factory) {}

		// adds value of predicate to hashmap
		PredicateIdentifierFactory::PredicateID addPredicate(const GreaterThanAttributeFilter<EventType, AttributeType> * greater_than_attribute_filter) {
			// first checking if identical predicate already exists => use same predicate id
			if (predicate_id_to_attribute_value_.find(greater_than_attribute_filter->get_constants()[0]) != predicate_id_to_attribute_value_.end()) {
				return predicate_id_to_attribute_value_[greater_than_attribute_filter->get_constants()[0]];
			}
			// no identical predicate exists, insert into hash map
			PredicateIdentifierFactory::PredicateID new_predicate_id = this->predicate_identifier_factory_->createPredicateIdentifier();
			predicate_id_to_attribute_value_.insert(std::pair<AttributeType, PredicateIdentifierFactory::PredicateID >(greater_than_attribute_filter->get_constants()[0], new_predicate_id));

			return new_predicate_id;
		}

		virtual void removePredicate(std::vector<bool>::size_type predicate_id) override {
			this->predicate_identifier_factory_->freeID(predicate_id);

			for (auto attributeValue_predicateID_pair_iter = predicate_id_to_attribute_value_.begin(); attributeValue_predicateID_pair_iter != predicate_id_to_attribute_value_.end(); ++attributeValue_predicateID_pair_iter) {
				if (attributeValue_predicateID_pair_iter->second == predicate_id) {
					predicate_id_to_attribute_value_.erase(attributeValue_predicateID_pair_iter);
				}
			}
		}

		// determines the fulfilled predicates which are in this index
		// And sets the "bit" in the fulfilled predicate vector to true
		virtual void determineMatchingPredicates(const EventType & event, std::vector<bool> & fulfilled_predicate_vector) override {
			// all predicates are fulfilled with a constant less than the value of attribute attribute_id_ in event

			auto attribute_value_iterator =  attributes_accessor_map.find(this->get_attribute_id());

			if (attribute_value_iterator == attributes_accessor_map.end()) {
				M2ETIS_THROW_API("GreaterThanPredicateIndex function determineMatchingPredicates", "Attribute not found in attribute map.");
			}

			auto itlow =predicate_id_to_attribute_value_.lower_bound((dynamic_cast<AttributeAccessor<EventType, AttributeType> *>(attribute_value_iterator->second.get()))->getAttributeValue(event));

			for (auto iter_predicates = predicate_id_to_attribute_value_.begin(); iter_predicates != itlow ; ++iter_predicates) {
				fulfilled_predicate_vector[iter_predicates->second] = true;
			}
		}

		virtual ~GreaterThanPredicateIndex() {}

	private:
		// hashmap as index structure to determine matching predicates efficiently
		// matches a value to the predicate identifier:
		// identical indexes get assigned the same predicate identifier
		std::map<AttributeType, PredicateIdentifierFactory::PredicateID > predicate_id_to_attribute_value_;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_GREATERTHANPREDICATEINDEX_H__ */

/**
 *  @}
 */
