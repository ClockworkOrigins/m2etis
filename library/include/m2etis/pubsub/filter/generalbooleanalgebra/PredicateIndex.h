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

#ifndef __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_PREDICATEINDEX_H__
#define __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_PREDICATEINDEX_H__

namespace m2etis {
namespace pubsub {
namespace filter {

	enum OperatorName {
		EQUALS,
		NOTEQUALS,
		GREATERTHAN,
		LESSTHAN
	}; // TODO: (Roland) already defined in FilterPredicate

	typedef int AttributeName; // TODO: (Roland) already defined in AttributeTypeInformation

	// per attribute_id and constraint (=attribute filter) one PredicateIndex
	template <typename EventType>
	class PredicateIndex {
	public:
		PredicateIndex(AttributeName attribute_id, OperatorName _operator, PredicateIdentifierFactory * predicate_identifier_factory) : attribute_id_(attribute_id), operator_(_operator), predicate_identifier_factory_(predicate_identifier_factory) {}

		// addPredicate function cannot be virtual because of AttributeType template parameter

		virtual void removePredicate(std::vector<bool>::size_type predicate_id) = 0;

		// TODO: (Roland) check, why vector cannnot be passed by reference
		virtual void determineMatchingPredicates(const EventType & event, std::vector<bool> & fulfilled_predicate_vector) = 0;

		virtual ~PredicateIndex() {}

		AttributeName get_attribute_id() const  {
			return attribute_id_;
		}

		OperatorName get_operator() const {
			return operator_;
		}

	private:
		AttributeName attribute_id_;

	protected:
		OperatorName operator_;
		PredicateIdentifierFactory * predicate_identifier_factory_;
	};

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_GENERALBOOLEANALGEBRA_PREDICATEINDEX_H__ */

/**
 *  @}
 */
