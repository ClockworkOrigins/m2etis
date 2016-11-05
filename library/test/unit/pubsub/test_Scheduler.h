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

#ifndef m2etis_scheduler_test_h
#define m2etis_scheduler_test_h

#include <chrono>
#include <cmath>
#include <thread>

#include "m2etis/pubsub/Scheduler.h"
#include "m2etis/util/RealTimeClock.h"

#include "gtest/gtest.h"

int func1_counter;

bool func1(long time, m2etis::util::Clock<m2etis::util::RealTimeClock> * cl, int counter) {
	EXPECT_LE(time, cl->getTime());
	EXPECT_LE(cl->getTime(), time + 3000);
	EXPECT_EQ(counter, func1_counter);
	func1_counter++;
	return true;
}

TEST(Scheduler, Once) {
	m2etis::util::Clock<m2etis::util::RealTimeClock> cl;
	m2etis::pubsub::Scheduler<m2etis::util::RealTimeClock> sched(cl);
	func1_counter = 0;
	sched.runOnce(50000, std::bind(&func1, cl.getTime() + 50000, &cl, 0), 0);
	sched.runOnce(100000, std::bind(&func1, cl.getTime() + 100000, &cl, 3), 0);
	sched.runOnce(150000, std::bind(&func1, cl.getTime() + 150000, &cl, 4), 1);
	sched.runOnce(75000, std::bind(&func1, cl.getTime() + 75000, &cl, 2), 0);
	sched.runOnce(75000, std::bind(&func1, cl.getTime() + 75000, &cl, 1), 1);
	for (int i = 0; i < 40; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	EXPECT_EQ(func1_counter, 5);
}

std::vector<int> jobs;

bool func2(long time, m2etis::util::Clock<m2etis::util::RealTimeClock> * cl, m2etis::pubsub::Scheduler<m2etis::util::RealTimeClock> * sched, int counter) {
	EXPECT_LE(time, cl->getTime());
	EXPECT_LE(cl->getTime(), time + 3000);
	jobs.push_back(counter);
	if (counter == 3) {
		sched->runRepeated(10000, std::bind(func2, cl->getTime() + 10000, cl, sched, 5), 0);
	}
	return false;
}

TEST(Scheduler, RepeatOnce) {
	m2etis::util::Clock<m2etis::util::RealTimeClock> cl;
	m2etis::pubsub::Scheduler<m2etis::util::RealTimeClock> sched(cl);
	jobs.clear();
	sched.runRepeated(50000, std::bind(func2, cl.getTime() + 50000, &cl, &sched, 0), 0);
	sched.runRepeated(100000, std::bind(func2, cl.getTime() + 100000, &cl, &sched, 1), 0);
	sched.runRepeated(150000, std::bind(func2, cl.getTime() + 150000, &cl, &sched, 2), 1);
	sched.runRepeated(75000, std::bind(func2, cl.getTime() + 75000, &cl, &sched, 3), 0);
	sched.runRepeated(75000, std::bind(func2, cl.getTime() + 75000, &cl, &sched, 4), 1);
	for (int i = 0; i < 40; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
    int b[] = {0, 4, 3, 5, 1, 2};
	std::vector<int> ref(b, b + sizeof(b) / sizeof(int));
	EXPECT_EQ(ref, jobs);
}

#endif /* m2etis_scheduler_test_h */
