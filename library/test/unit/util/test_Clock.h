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

#ifndef __M2ETIS_CLOCK_TEST_H__
#define __M2ETIS_CLOCK_TEST_H__

#include <chrono>
#include <cmath>
#include <thread>

#include "m2etis/util/Clock.h"
#include "m2etis/util/RealTimeClock.h"

TEST(Clock, RealTime) {
    	m2etis::util::Clock<m2etis::util::RealTimeClock> clock;
    	uint64_t lastTime = clock.getTime();
    	lastTime += 2000;
    	for(int i = 1; i < 10; ++i) {
    		std::this_thread::sleep_for(std::chrono::milliseconds(50));
    		// set the value on the right to anything that will work. As long it is << 50.000
    		// the test is ok. It's just .. this loop takes more than 0 microsecssecs
    		uint64_t a = clock.getTime();
    		EXPECT_LT(std::abs(a - lastTime), 51000);
    		lastTime = a;
    	}

    	EXPECT_EQ(clock.registerTimer(), 0);
    	EXPECT_EQ(clock.registerTimer(), 1);
    	EXPECT_EQ(clock.registerTimer(), 2);
}

uint64_t sT;
bool done;

void func(m2etis::util::Clock<m2etis::util::RealTimeClock> * c) {
	for(int i = 0; i < 10; ++i) {
		// wait for some time
		uint64_t wT = 50000;
		c->waitForTime(0, sT + wT);
		uint64_t t = c->getTime();
		EXPECT_LE(sT + wT, t);
		sT = t;
	}
	done = true;
}

TEST(Clock, Notifies) {
    	m2etis::util::Clock<m2etis::util::RealTimeClock> * clock = new m2etis::util::Clock<m2etis::util::RealTimeClock>();
    	int ind = clock->registerTimer();
    	EXPECT_EQ(ind, 0);
    	sT = clock->getTime();
    	done = false;
    	std::thread(std::bind(func, clock)).detach();
    	for (int i = 0; i < 20; ++i) {
    		std::this_thread::sleep_for(std::chrono::milliseconds(20));
    	}
    	EXPECT_FALSE(done);
    	for (int i = 0; i < 30; ++i) {
    		std::this_thread::sleep_for(std::chrono::milliseconds(20));
    	}
    	EXPECT_TRUE(done);
    	delete clock;
}

#endif /* __M2ETIS_CLOCK_TEST_H__ */
