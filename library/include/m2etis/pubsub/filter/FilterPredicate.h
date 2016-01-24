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

#ifndef __M2ETIS_PUBSUB_FILTER_FILTERPREDICATE_H__
#define __M2ETIS_PUBSUB_FILTER_FILTERPREDICATE_H__

#include <functional>
#include <map>
#include <set>
#include <string>

#include "m2etis/TemplateHelper.h" // for Int2Type

#include "m2etis/pubsub/filter/AttributeAccessor.h"
#include "m2etis/pubsub/filter/AttributeAccessorBasic.h"
#include "m2etis/pubsub/filter/AttributeTypeInformation.h"

#include "m2etis/pubsub/filter/filterexpressions/AndExp.h"
#include "m2etis/pubsub/filter/filterexpressions/AttributeFilter.h"
#include "m2etis/pubsub/filter/filterexpressions/EqualsAttributeFilter.h"
#include "m2etis/pubsub/filter/filterexpressions/FalsePredicate.h"
#include "m2etis/pubsub/filter/filterexpressions/FilterExp.h"
#include "m2etis/pubsub/filter/filterexpressions/GreaterThanAttributeFilter.h"
#include "m2etis/pubsub/filter/filterexpressions/LessThanAttributeFilter.h"
#include "m2etis/pubsub/filter/filterexpressions/NotEqualsAttributeFilter.h"
#include "m2etis/pubsub/filter/filterexpressions/OrExp.h"
#include "m2etis/pubsub/filter/filterexpressions/Predicate.h"
#include "m2etis/pubsub/filter/filterexpressions/TruePredicate.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/make_shared.hpp"
#include "boost/serialization/export.hpp"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/variant.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"

namespace m2etis {
namespace pubsub {

	M2ETIS_API extern std::map<filter::AttributeName, std::shared_ptr<filter::AttributeAccessor_Basic> > attributes_accessor_map; // TODO: (Roland) make non-static

	enum OperatorName {
		EQUALS, NOTEQUALS, GREATERTHAN, LESSTHAN
	};

namespace filter {

	template <typename EventType, typename AttributeType> class AttributeFilter;
	template <typename EventType, typename AttributeType> class EqualsAttributeFilter;
	template <typename EventType, typename AttributeType> class NotEqualsAttributeFilter;
	template <typename EventType, typename AttributeType> class GreaterThanAttributeFilter;
	template <typename EventType, typename AttributeType> class LessThanAttributeFilter;

	template <typename EventType, typename NetworkType> class DecisionTreeFilter;
	template <typename EventType, typename NetworkType> class GeneralBooleanExpressionsFilter;

	template <typename EventType, typename NetworkType> class DecisionTreePreProcessVisitor;
	template <typename EventType, typename NetworkType> class GeneralBooleanExpressionsPreProcessVisitor;

	// the following operator overloading and creator functions allow writing
	// boolean expressions like:
// 	boost::make_shared<m2etis::pubsub::filter::OrExp<Position> > (
// 	    	    			(Equals<Position, int>(POSITION_X, {90}) ||
// 	    	    					Equals<Position, std::string>(POSITION_REGION, {"Ansbach"})
//
// 	    	    					)

	// instead of the following more complex syntax:
	// x = 1 or y = 1:
	// auto static_predicate2 = std::make_shared<OrExp<Position> > (
	// 		std::make_shared<EqualsAttributeFilter<Position, int> >(&Position::x, std::vector<int>( {1})),
	// 		std::make_shared<EqualsAttributeFilter<Position, int> >(&Position::y, std::vector<int>( {1})));


	// operator overloading (easier notation):
	template <typename BooleanExpType1, typename BooleanExpType2> OrExp<typename BooleanExpType1::schema> operator||(const BooleanExpType1 & operand1, const BooleanExpType2 & operand2) {
		return OrExp<typename BooleanExpType1::schema>(boost::make_shared<BooleanExpType1>(operand1), boost::make_shared<BooleanExpType2>(operand2));
	}
	template <typename BooleanExpType1, typename BooleanExpType2> AndExp<typename BooleanExpType1::schema> operator&&(const BooleanExpType1 & operand1, const BooleanExpType2 & operand2) {
		return AndExp<typename BooleanExpType1::schema>(boost::make_shared<BooleanExpType1>(operand1), boost::make_shared<BooleanExpType2>(operand2));
	}

	// creator functions for type deduction and easier notation:
	template <class EventType, class AttributeType> EqualsAttributeFilter<EventType, AttributeType> Equals(AttributeName attribute_id, const AttributeType & constants) {
		return EqualsAttributeFilter<EventType, AttributeType>(attribute_id, constants);
	}

	template <class EventType, class AttributeType> NotEqualsAttributeFilter<EventType, AttributeType> NotEquals(AttributeName attribute_id, const AttributeType & constants) {
		return NotEqualsAttributeFilter<EventType, AttributeType>(attribute_id, constants);
	}

	template <class EventType, class AttributeType> LessThanAttributeFilter<EventType, AttributeType> LessThan(AttributeName attribute_id, const AttributeType & constants) {
		return LessThanAttributeFilter<EventType, AttributeType>(attribute_id, constants);
	}

	template <class EventType, class AttributeType> GreaterThanAttributeFilter<EventType, AttributeType> GreaterThan(AttributeName attribute_id, const AttributeType & constants) {
		return GreaterThanAttributeFilter<EventType, AttributeType>(attribute_id, constants);
	}

} /* namespace filter */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_FILTER_FILTERPREDICATE_H__ */

/**
 * @}
 */
