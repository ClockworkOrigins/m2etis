/*
 * Copyright  2011-2012  Prof. Dr. Richard Lenz, Thomas Fischer
 *
 * Licensed  under  the  Apache  License,  Version  2.0  (the  "License");
 * you  may  not  use  this  file  except  in  compliance  with  the  License.
 * You  may  obtain  a  copy  of  the  License  at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless  required  by  applicable  law  or  agreed  to  in  writing,  software
 * distributed  under  the  License  is  distributed  on  an  "AS IS"  BASIS,
 * WITHOUT  WARRANTIES  OR  CONDITIONS  OF  ANY  KIND,  either  express  or  implied.
 * See  the  License  for  the  specific  language  governing  permissions  and
 * limitations  under  the  License.
 *
 */

/**
 * \addtogroup core
 * @ {
 */

#ifndef __M2ETIS_UTIL_CLOCK_H__
#define __M2ETIS_UTIL_CLOCK_H__

#include <climits>
#include <map>

#include "m2etis/util/Logger.h"

#include "boost/thread.hpp"

namespace m2etis {
namespace util {

	template<class Updater>
	class Clock : public Updater {
	public:
		Clock() : Updater(boost::bind(&Clock::Update, this)), timer_(), lock_(), usedIds_(0), systemTime_(0), running_(true), offset_(0) {
		}

		~Clock() {
			Updater::Stop();
			running_ = false;
			boost::mutex::scoped_lock l(lock_);
			for (std::map<uint64_t, std::pair<uint64_t, boost::condition_variable *>>::iterator it = timer_.begin(); it != timer_.end(); ++it) {
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
			return uint64_t(int64_t(systemTime_) + offset_);
		}

		/**
		 * \brief Will return the time since the Clock has been started
		 *
		 * \return Current time in microseconds
		 */
		uint64_t getRealTime() const {
			return systemTime_;
		}

		void Update() {
			if (!running_) {
				return;
			}
			// Get current time.
			boost::mutex::scoped_lock sl(lock_);
			systemTime_ = Updater::getCurrentTime(systemTime_);
			notifyTimer();
		}

		uint64_t registerTimer() {
			std::pair<uint64_t, boost::condition_variable *> p(std::make_pair(UINT64_MAX, new boost::condition_variable()));

			lock_.lock();
			uint64_t id = usedIds_++;
			timer_[id] = p;
			lock_.unlock();

			return id;
		}

		void unregisterTimer(uint64_t timerID) {
			lock_.lock();
			delete timer_[timerID].second;
			timer_.erase(timerID);
			lock_.unlock();
		}

		void updateWaitTime(uint64_t timerID, uint64_t time) {
			lock_.lock();
			if (time <= systemTime_ + offset_) {
				timer_[timerID].second->notify_all();
				lock_.unlock();
				return;
			}
			timer_[timerID].first = time;
			lock_.unlock();
		}

		bool waitForTime(uint64_t timerID, uint64_t time) {
			if (!running_) {
				return false;
			}
			boost::mutex::scoped_lock lock(lock_); // Is a scoped lock
			if (time <= systemTime_ + offset_) {
				return true;
			}
			timer_[timerID].first = time;
			timer_[timerID].second->wait(lock);
			return running_;
		}

		bool isRunning() {
			return running_;
		}

		void setOffset(int64_t offset) {
			offset_ = offset;
		}

	private:
		Clock(const Clock &) = delete;
		Clock & operator=(const Clock &) = delete;

		//        id            wakeuptime      variable
		std::map<uint64_t, std::pair<uint64_t, boost::condition_variable *> > timer_;

		boost::mutex lock_;
		uint64_t usedIds_;

		// last system time
		volatile uint64_t systemTime_;

		volatile bool running_;

		volatile int64_t offset_;

		void notifyTimer() {
			for (std::map<uint64_t, std::pair<uint64_t, boost::condition_variable *>>::iterator it = timer_.begin(); it != timer_.end(); ++it) {
				if (systemTime_ + offset_ >= it->second.first) {
					it->second.first = UINT64_MAX;
					it->second.second->notify_all();
				}
			}
		}
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_CLOCK_H__ */

/**
 * @}
 */
