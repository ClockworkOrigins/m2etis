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

#define _GLIBCXX_USE_NANOSLEEP // needed for sleep_for, see http://stackoverflow.com/questions/4438084/stdthis-threadsleep-for-and-gcc

#include "m2etis/util/RealTimeClock.h"

#include <thread>

#include "m2etis/parameters/QueueParameters.h"

namespace m2etis {
namespace util {

	RealTimeClock::RealTimeClock(const std::function<void(void)> & f) : startTime_(std::chrono::high_resolution_clock::now()), update_(f), _running(true), thread_() {
	}

	RealTimeClock::~RealTimeClock() {
		Stop();
	}

	void RealTimeClock::Init() {
		thread_ = std::thread(std::bind(&RealTimeClock::clockUpdater, this));
	}

	uint64_t RealTimeClock::getCurrentTime(uint64_t) {
		return uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime_).count());
	}

	void RealTimeClock::Stop() {
		if (_running) {
			_running = false;
			thread_.join();
		}
	}

	void RealTimeClock::clockUpdater() {
		while (_running) {
			std::this_thread::sleep_for(std::chrono::microseconds(parameters::CLOCK_UPDATERATE));
			if (_running) {
				update_();
			}
		}
	}

} /* namespace util */
} /* namespace m2etis */
