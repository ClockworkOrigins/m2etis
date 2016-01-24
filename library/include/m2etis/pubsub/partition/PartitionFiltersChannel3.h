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

#ifndef m2etis_PartitionFiltersChannel3_h
#define m2etis_PartitionFiltersChannel3_h

#include "m2etis/pubsub/partition/BruteForcePartition.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"
#include "m2etis/config/GeneratedEventTypes.h"

using m2etis::pubsub::filter::Position;

using m2etis::pubsub::filter::EqualsAttributeFilter;
using m2etis::pubsub::filter::GreaterThanAttributeFilter;
using m2etis::pubsub::filter::LessThanAttributeFilter;
using m2etis::pubsub::filter::AndExp;
using m2etis::pubsub::filter::OrExp;

using m2etis::pubsub::filter::NotEquals;
using m2etis::pubsub::filter::Equals;
using m2etis::pubsub::filter::LessThan;
using m2etis::pubsub::filter::GreaterThan;

using m2etis::pubsub::filter::AttributeName;
using m2etis::pubsub::filter::AttributeAccessor_Basic;
using m2etis::pubsub::filter::AttributeAccessor;

enum attribute_names {
	POSITION_X,
	POSITION_Y,
	POSITION_REGION,
	BOOK_TITLE,
	BOOK_PRICE,
	BOOK_CONDITION
};

template <typename EventType>
class PartitionFiltersChannel3 : public m2etis::pubsub::partition::BruteForcePartition<EventType> {
public:
    PartitionFiltersChannel3() : m2etis::pubsub::partition::BruteForcePartition<EventType>( {
    	// filter partition 0: x < 1 and y < 1 and (x > 0 or y > 0 or x = 0 or y = 0))
    	// (short notation with operator overloading and conversion operator to shared_ptr of AndExpr)
    	LessThan<Position, int>(POSITION_X, 1) && LessThan<Position, int>(POSITION_Y, 1) && (GreaterThan<Position, int>(POSITION_X, 0) || Equals<Position, int>(POSITION_X, 0) || GreaterThan<Position, int>(POSITION_Y, 0) || Equals<Position, int>(POSITION_Y, 0)),

		// partition 1:
		// x = 1 or y = 1
		// (extended notation)
		boost::make_shared<OrExp<Position> >(boost::make_shared<EqualsAttributeFilter<Position, int> >(POSITION_X, 1), boost::make_shared<EqualsAttributeFilter<Position, int> >(POSITION_Y, 1)),

		// partition 2:
		// x > 1 or y > 1
		boost::make_shared<OrExp<Position> >(boost::make_shared<GreaterThanAttributeFilter<Position, int> >(POSITION_X, 1), boost::make_shared<GreaterThanAttributeFilter<Position, int> >(POSITION_Y, 1)),

		// partition3:
		// (x < 0 and y < 0)
		boost::make_shared<AndExp<Position> >(boost::make_shared<LessThanAttributeFilter<Position, int> >(POSITION_X, 0), boost::make_shared<LessThanAttributeFilter<Position, int> >(POSITION_Y, 0))
    } ) {}
};

#endif // m2etis_PartitionFiltersChannel3_h

/**
 * @}
 */
