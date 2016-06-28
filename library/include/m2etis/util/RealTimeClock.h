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
 * \addtogroup util
 * @{
 */

#ifndef __M2ETIS_UTIL_REALTIMECLOCK_H__
#define __M2ETIS_UTIL_REALTIMECLOCK_H__

#include <chrono>
#include <thread>

#include "boost/function.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace util {

	/**
	 * \brief This Clock handles the real time
	 */
	class RealTimeClock {
	public:
		/**
		 * \brief initializes a worker thread calling given function when time changed
		 */
		explicit RealTimeClock(const std::function<void(void)> & f);

		/**
		 * \brief destructor
		 */
		virtual ~RealTimeClock();

		/**
		 * \brief returns current time in microseconds
		 */
		uint64_t getCurrentTime(uint64_t lastTime);

		/**
		 * \brief stops the clock
		 */
		void Stop();

	protected:
		void Init();

	private:
		const std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;

		std::function<void(void)> update_;

		volatile bool _running;

		std::thread thread_;

		void clockUpdater();

		RealTimeClock & operator=(const RealTimeClock & o) = delete;
		RealTimeClock(const RealTimeClock & o) = delete;
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_REALTIMECLOCK_H__ */

/**
 * @}
 */
