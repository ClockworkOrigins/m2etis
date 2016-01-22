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

#include "m2etis/util/RealTimeClock.h"
#include "m2etis/util/Logger.h"

#include "m2etis/parameters/QueueParameters.h"

namespace m2etis {
namespace util {

	RealTimeClock::RealTimeClock(const boost::function<void(void)> & f) : startTime_(boost::posix_time::microsec_clock::universal_time()), update_(f), _running(true), thread_() {
	}

	RealTimeClock::~RealTimeClock() {
		Stop();
	}

	void RealTimeClock::Init() {
		thread_ = boost::thread(boost::bind(&RealTimeClock::clockUpdater, this));
	}

	uint64_t RealTimeClock::getCurrentTime(uint64_t) {
		return uint64_t(boost::posix_time::time_period(startTime_, boost::posix_time::microsec_clock::universal_time()).length().total_microseconds());
	}

	void RealTimeClock::Stop() {
		_running = false;
		thread_.interrupt();
		thread_.join();
	}

	void RealTimeClock::clockUpdater() {
		while (_running) {
			boost::this_thread::sleep(boost::posix_time::microseconds(parameters::CLOCK_UPDATERATE));
			if (_running) {
				update_();
			}
		}
	}

} /* namespace util */
} /* namespace m2etis */
