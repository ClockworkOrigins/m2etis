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

#ifndef __M2ETIS_UTIL_CLOCK_H__
#define __M2ETIS_UTIL_CLOCK_H__

#ifdef ANDROID
	#ifndef UINT64_MAX
		#define UINT64_MAX (__UINT64_C(18446744073709551615))
	#endif
#endif

#include <climits>
#include <condition_variable>
#include <map>

#include "m2etis/util/Logger.h"

namespace m2etis {
namespace util {

	/**
	 * \brief Clock for time handling, Updater is the implementation of the time, like a RealTimeClock measuring real time
	 */
	template<class Updater>
	class Clock : public Updater {
	public:
		/**
		 * \brief initializes Clock and propagates Update method to Updater
		 */
		Clock() : Updater(std::bind(&Clock::Update, this)), timer_(), lock_(), usedIds_(0), systemTime_(0), running_(true), offset_(0) {
			Updater::Init();
		}

		/**
		 * \brief destructor
		 */
		~Clock() {
			Updater::Stop();
			running_ = false;
			std::lock_guard<std::mutex> lg(lock_);
			for (std::map<uint64_t, std::pair<uint64_t, std::condition_variable *>>::iterator it = timer_.begin(); it != timer_.end(); ++it) {
				it->second.second->notify_all();
				// FIXME: but there musn't be any remaing waiting processes...
				delete it->second.second;
			}
			timer_.clear();
		}

		/**
		 * \brief Will return the time since the Clock on the rendezvouz node has started
		 *
		 * \return Current time in microseconds
		 */
		uint64_t getTime() const {
			return systemTime_ + offset_;
		}

		/**
		 * \brief Will return the real time since the Clock has been started
		 *
		 * \return Current time in microseconds
		 */
		uint64_t getRealTime() const {
			return systemTime_;
		}

		/**
		 * \brief returns a new ID for a timer
		 */
		uint64_t registerTimer() {
			std::pair<uint64_t, std::condition_variable *> p(std::make_pair(UINT64_MAX, new std::condition_variable()));

			std::lock_guard<std::mutex> lg(lock_);
			uint64_t id = usedIds_++;
			timer_[id] = p;

			return id;
		}

		/**
		 * \brief frees a timer ID
		 */
		void unregisterTimer(uint64_t timerID) {
			std::lock_guard<std::mutex> lg(lock_);
			delete timer_[timerID].second;
			timer_.erase(timerID);
		}

		/**
		 * \brief updates the time a given timer has to wait
		 */
		void updateWaitTime(uint64_t timerID, uint64_t time) {
			std::lock_guard<std::mutex> lg(lock_);
			if (time <= systemTime_ + offset_) {
				timer_[timerID].second->notify_all();
				return;
			}
			timer_[timerID].first = time;
		}

		/**
		 * \brief timer with timerID will wait for time and then be unlocked again
		 */
		bool waitForTime(uint64_t timerID, uint64_t time) {
			if (!running_) {
				return false;
			}
			std::unique_lock<std::mutex> ul(lock_);
			if (time <= systemTime_ + offset_) {
				return true;
			}
			timer_[timerID].first = time;
			timer_[timerID].second->wait(ul);
			return running_;
		}

		/**
		 * \brief gets state of the Clock
		 */
		bool isRunning() {
			return running_;
		}

		/**
		 * \brief sets offset of this Clock to adjust times over network
		 */
		void setOffset(uint64_t offset) {
			offset_ = offset;
		}

	private:
		//        id            wakeuptime      variable
		std::map<uint64_t, std::pair<uint64_t, std::condition_variable *>> timer_;

		std::mutex lock_;
		uint64_t usedIds_;

		// last system time
		volatile uint64_t systemTime_;

		volatile bool running_;

		volatile uint64_t offset_;

		void Update() {
			if (!running_) {
				return;
			}
			// Get current time.
			std::lock_guard<std::mutex> lg(lock_);
			systemTime_ = Updater::getCurrentTime(systemTime_);
			notifyTimer();
		}

		void notifyTimer() {
			for (std::map<uint64_t, std::pair<uint64_t, std::condition_variable *>>::iterator it = timer_.begin(); it != timer_.end(); ++it) {
				if (systemTime_ + offset_ >= it->second.first) {
					it->second.first = UINT64_MAX;
					it->second.second->notify_all();
				}
			}
		}

		Clock(const Clock &) = delete;
		Clock & operator=(const Clock &) = delete;
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_CLOCK_H__ */

/**
 * @}
 */
