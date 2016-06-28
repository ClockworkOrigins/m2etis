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

#ifndef __M2ETIS_TEST_UTIL_DOUBLEBUFFERQUEUE_H__
#define __M2ETIS_TEST_UTIL_DOUBLEBUFFERQUEUE_H__

#include "m2etis/util/DoubleBufferQueue.h"

#include "boost/thread.hpp"

using m2etis::util::DoubleBufferQueue;

TEST(DoubleBufferQueue, Simple) {
	DoubleBufferQueue<int, false, false> q;
	EXPECT_TRUE(q.empty());
	EXPECT_EQ(q.size(), 0);
}

TEST(DoubleBufferQueue, Pushing) {
	DoubleBufferQueue<int, false, false> q;
	for(int i = 0; i < 10; ++i) {
		q.push(i);
		EXPECT_FALSE(q.empty());
		EXPECT_EQ(q.size(), i + 1);
	}
}

TEST(DoubleBufferQueue, PushPop) {
	DoubleBufferQueue<int, false, false> q;
	for(int i = 0; i < 10; ++i) {
		q.push(i);
		EXPECT_FALSE(q.empty());
		EXPECT_EQ(q.size(), i + 1);
		EXPECT_EQ(q.front(), 0);
	}
	for(int i = 0; i < 10; ++i) {
		EXPECT_EQ(q.front(), i);
		q.pop();
		EXPECT_EQ(q.size(), 9 - i);
	}
	EXPECT_TRUE(q.empty());
}

TEST(DoubleBufferQueue, Poll) {
	DoubleBufferQueue<int, false, false> q;
	for(int i = 0; i < 10; ++i) {
		q.push(i);
		EXPECT_FALSE(q.empty());
		EXPECT_EQ(q.size(), i + 1);
		EXPECT_EQ(q.front(), 0);
	}
	for(int i = 0; i < 10; ++i) {
		int a = q.poll();
		EXPECT_EQ(a, i);
		EXPECT_EQ(q.size(), 9 - i);
	}
	EXPECT_TRUE(q.empty());
}

TEST(DoubleBufferQueue, PushPoll2) {
	DoubleBufferQueue<int, false, false> q;
	for(int i = 0; i < 5; ++i) {
		q.push(7);
		EXPECT_EQ(q.size(), 1);
		q.push(56);
		EXPECT_EQ(q.size(), 2);
		EXPECT_EQ(q.poll(), 7);
		EXPECT_EQ(q.size(), 1);
		q.push(23);
		EXPECT_EQ(q.size(), 2);
		EXPECT_EQ(q.poll(), 56);
		EXPECT_EQ(q.poll(), 23);
		EXPECT_TRUE(q.empty());
	}
}

TEST(DoubleBufferQueue, Clear) {
	DoubleBufferQueue<int, false, false> q;
	q.clear();
	for(int i = 0; i < 5; ++i) {
		q.push(13);
		q.push(37);
		q.push(23);
		q.push(7);
		q.push(42);
		EXPECT_EQ(q.size(), 5);
		EXPECT_FALSE(q.empty());
		q.clear();
		EXPECT_EQ(q.size(), 0);
		EXPECT_TRUE(q.empty());
	}
}

TEST(DoubleBufferQueue, Exception) {
	DoubleBufferQueue<int, false, false> q;
	ASSERT_THROW(q.pop(), m2etis::util::ApiException);
	ASSERT_THROW(q.front(), m2etis::util::ApiException);
	ASSERT_THROW(q.poll(), m2etis::util::ApiException);
	q.push(23);
	q.pop();
    	ASSERT_THROW(q.pop(), m2etis::util::ApiException);
    	ASSERT_THROW(q.front(), m2etis::util::ApiException);
	ASSERT_THROW(q.poll(), m2etis::util::ApiException);
}

void pusher(DoubleBufferQueue<int, true, true> * q, int amount, int value) {
	for(int i = 0; i < amount; ++i) {
		q->push(value);
	}
}

void popper(DoubleBufferQueue<int, true, true> * qFrom, DoubleBufferQueue<int, true, false> * qTo, int amount) {
	for(int i = 0; i < amount; ++i) {
		try {
			int a = qFrom->poll();
			qTo->push(a);
		} catch(m2etis::util::ApiException m) {
			i--;
		}
	}
}

TEST(DoubleBufferQueue, StressTest) {
	DoubleBufferQueue<int, true, true> q1;
	DoubleBufferQueue<int, true, false> q2;
	std::vector<std::thread *> v;
	for(int i = 0; i < 40; ++i) {
		v.push_back(new std::thread(std::bind(pusher, &q1, 10000, i)));
	}
	for(int i = 0; i < 80; ++i) {
		v.push_back(new std::thread(std::bind(popper, &q1, &q2, 5000)));
	}
	std::vector<int> counts(40);
	for(unsigned int i = 0; i < 120; ++i) {
		v[i]->join();
		delete v[i];
	}
	for(int i = 0; i < 40 * 10000; ++i) {
		counts[static_cast<size_t>(q2.poll())]++;
	}
	for(unsigned int i = 0; i < 40; ++i) {
		EXPECT_EQ(10000, counts[i]);
	}
	EXPECT_TRUE(q1.empty());
	EXPECT_TRUE(q2.empty());
}

#endif /* __M2ETIS_TEST_UTIL_DOUBLEBUFFERQUEUE_H__ */

