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

#ifndef PREDICATE_TEST_H_
#define PREDICATE_TEST_H_

#include "m2etis/pubsub/filter/events/Position.h"
#include "m2etis/pubsub/filter/FilterPredicate.h"

using namespace m2etis::pubsub::filter;

enum attribute_names {
	POSITION_X,
	POSITION_Y,
	POSITION_REGION,
	BOOK_TITLE,
	BOOK_PRICE,
	BOOK_CONDITION
};

TEST(PredicateTest, PredicateComparisonTest) {
	auto f1 = LessThan<Position, int>(POSITION_X, 1) && LessThan<Position, int>(POSITION_Y, 1) && (GreaterThan<Position, int>(POSITION_X, 0) || Equals<Position, int>(POSITION_X, 0) || GreaterThan<Position, int>(POSITION_Y, 0) || Equals<Position, int>(POSITION_Y, 0));

	// partition 1:
	// x = 1 or y = 1
	// (extended notation)
	auto f2 = boost::make_shared<OrExp<Position> >(boost::make_shared<EqualsAttributeFilter<Position, int> >(POSITION_X, 1), boost::make_shared<EqualsAttributeFilter<Position, int> >(POSITION_Y, 1));

	// partition 2:
	// x > 1 or y > 1
	auto f3 = boost::make_shared<OrExp<Position> >(boost::make_shared<GreaterThanAttributeFilter<Position, int> >(POSITION_X, 1), boost::make_shared<GreaterThanAttributeFilter<Position, int> >(POSITION_Y, 1));

	// partition3:
	// (x < 0 and y < 0)
	auto f4 = boost::make_shared<AndExp<Position> >(boost::make_shared<LessThanAttributeFilter<Position, int> >(POSITION_X, 0), boost::make_shared<LessThanAttributeFilter<Position, int> >(POSITION_Y, 0));

	auto f5 = LessThan<Position, int>(POSITION_X, 1) && LessThan<Position, int>(POSITION_Y, 1) && (GreaterThan<Position, int>(POSITION_X, 0) || Equals<Position, int>(POSITION_X, 0) || GreaterThan<Position, int>(POSITION_Y, 0) || Equals<Position, int>(POSITION_Y, 0));

	// partition 1:
	// x = 1 or y = 1
	// (extended notation)
	auto f6 = boost::make_shared<OrExp<Position> >(boost::make_shared<EqualsAttributeFilter<Position, int> >(POSITION_X, 1), boost::make_shared<EqualsAttributeFilter<Position, int> >(POSITION_Y, 1));

	// partition 2:
	// x > 1 or y > 1
	auto f7 = boost::make_shared<OrExp<Position> >(boost::make_shared<GreaterThanAttributeFilter<Position, int> >(POSITION_X, 1), boost::make_shared<GreaterThanAttributeFilter<Position, int> >(POSITION_Y, 1));

	// partition3:
	// (x < 0 and y < 0)
	auto f8 = boost::make_shared<AndExp<Position> >(boost::make_shared<LessThanAttributeFilter<Position, int> >(POSITION_X, 0), boost::make_shared<LessThanAttributeFilter<Position, int> >(POSITION_Y, 0));

	EXPECT_NE(f1, *f2);
	EXPECT_NE(f1, *f3);
	EXPECT_NE(f1, *f4);
	EXPECT_NE(*f2, *f3);
	EXPECT_NE(*f2, *f4);
	EXPECT_NE(*f3, *f4);
	EXPECT_EQ(true, f1 == f5 && *f2 == *f6 && *f3 == *f7 && *f4 == *f8);

	EXPECT_EQ(true, std::hash<FilterExp<Position> >()(f1) == std::hash<FilterExp<Position> >()(f5) && std::hash<FilterExp<Position> >()(*f2) == std::hash<FilterExp<Position> >()(*f6) && std::hash<FilterExp<Position> >()(*f3) == std::hash<FilterExp<Position> >()(*f7) && std::hash<FilterExp<Position> >()(*f4) == std::hash<FilterExp<Position> >()(*f8));

	boost::shared_ptr<m2etis::pubsub::filter::AndExp<Position> > subscribe_filter1 = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(boost::make_shared<m2etis::pubsub::filter::AndExp<Position> > (boost::make_shared<m2etis::pubsub::filter::EqualsAttributeFilter<Position, std::string> >(POSITION_REGION, "Erlangen"), boost::make_shared<m2etis::pubsub::filter::LessThanAttributeFilter<Position, int> >(POSITION_X, 5)), boost::make_shared<m2etis::pubsub::filter::LessThanAttributeFilter<Position, int> >(POSITION_X, 5));

	boost::shared_ptr<m2etis::pubsub::filter::AndExp<Position> > subscribe_filter2 = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> > (boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(boost::make_shared<m2etis::pubsub::filter::EqualsAttributeFilter<Position, std::string> >(POSITION_REGION, "Erlangen"), boost::make_shared<m2etis::pubsub::filter::LessThanAttributeFilter<Position, int> >(POSITION_X, 5)), boost::make_shared<m2etis::pubsub::filter::LessThanAttributeFilter<Position, int> >(POSITION_X, 5));

	boost::shared_ptr<m2etis::pubsub::filter::AndExp<Position> > subscribe_filter3 = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(boost::make_shared<m2etis::pubsub::filter::EqualsAttributeFilter<Position, std::string> >(POSITION_REGION, "Erlangen"), boost::make_shared<m2etis::pubsub::filter::LessThanAttributeFilter<Position, int> >(POSITION_X, 5)), boost::make_shared<m2etis::pubsub::filter::GreaterThanAttributeFilter<Position, int> >(POSITION_X, 5));

	boost::shared_ptr<m2etis::pubsub::filter::AndExp<Position> > subscribe_filter4 = boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(boost::make_shared<m2etis::pubsub::filter::AndExp<Position> >(boost::make_shared<m2etis::pubsub::filter::EqualsAttributeFilter<Position, std::string> >(POSITION_REGION, "Erlangen"), boost::make_shared<m2etis::pubsub::filter::LessThanAttributeFilter<Position, int> >(POSITION_X, 5)), boost::make_shared<m2etis::pubsub::filter::GreaterThanAttributeFilter<Position, int> >(POSITION_X, 5));

	EXPECT_EQ(*subscribe_filter1, *subscribe_filter2);
	EXPECT_EQ(*subscribe_filter3, *subscribe_filter4);

	EXPECT_NE(*subscribe_filter1, *subscribe_filter3);
}

#endif /* PREDICATE_TEST_H_ */
