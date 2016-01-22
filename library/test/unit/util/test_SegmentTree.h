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

#ifndef __M2ETIS_TEST_UTIL_SEGMENTTREE_H__
#define __M2ETIS_TEST_UTIL_SEGMENTTREE_H__

#include "m2etis/util/SegmentTree.h"

using m2etis::util::SegmentTree;

TEST(SegmentTree, Basic) {
	SegmentTree<int> s;

	EXPECT_EQ(0, s.size());

	s.insert(1);

	EXPECT_EQ(1, s.size());

	s.insert(2);

	EXPECT_EQ(1, s.size());

	s.insert(4);

	EXPECT_EQ(2, s.size());

	s.insert(3);

	EXPECT_EQ(1, s.size());

	s.insert(-1);

	EXPECT_EQ(2, s.size());

	s.insert(0);

	EXPECT_EQ(1, s.size());

	s.insert(10);

	EXPECT_EQ(2, s.size());

	s.insert(6);

	EXPECT_EQ(3, s.size());

	s.insert(7);

	EXPECT_EQ(3, s.size());

	s.insert(9);

	EXPECT_EQ(3, s.size());

	s.insert(8);

	EXPECT_EQ(2, s.size());

	s.insert(5);

	EXPECT_EQ(1, s.size());

	for (int i = -1; i <= 10; ++i) {
		EXPECT_TRUE(s.contains(i));
	}

	EXPECT_EQ(12, s.count());
}

#endif /* __M2ETIS_TEST_UTIL_SEGMENTTREE_H__ */
