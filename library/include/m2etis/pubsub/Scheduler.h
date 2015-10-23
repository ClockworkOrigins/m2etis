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

#ifndef __M2ETIS_PUBSUB_SCHEDULER_H__
#define __M2ETIS_PUBSUB_SCHEDULER_H__

#include <atomic>
#include <cstdint>
#include <queue>

#include "m2etis/util/Clock.h"

#include "boost/thread.hpp"

namespace m2etis {
namespace pubsub {

	template<class ClockUpdater>
	class Scheduler {
	public:
		typedef struct Job {
			Job(const boost::function<bool(void)> & f, uint64_t t, int16_t p, uint64_t d, uint64_t i = UINT64_MAX) : func(f), time(t), priority(p), interval(i), id(d) {
			}
			boost::function<bool(void)> func;
			uint64_t time;
			int16_t priority; // the higher, the better
			uint64_t interval;
			uint64_t id;

			bool operator<(const Job & other) const {
				if (time != other.time) {
					return time > other.time;
				}
				if (priority != other.priority) {
					return priority < other.priority;
				}
				return interval > other.interval;
			}
		} Job;

		explicit Scheduler(util::Clock<ClockUpdater> & c) : running_(true), clock_(c), queue_(), lock_()
#ifndef WITH_SIM
		, worker_(boost::bind(&Scheduler<ClockUpdater>::worker, this))
#else
		, worker_()
#endif
		, id_(), stopMap_() {
		}

		~Scheduler() {
			// FIXME: this won't be enough since the object doesn't exist anymore, but the thread hasn't shut down yet
		}

		/**
		 * \brief adds new job running only once
		 *
		 * \param[in] time time after which this job will be scheduled (in microseconds)
		 * \param[in] func function to be called when scheduled
		 * \param[in] priority priority of this job
		 */
		uint64_t runOnce(uint64_t time, const boost::function<bool(void)> & func, int16_t priority) {
			// TODO: if first element in queue, adjust waitingtime
			Job j(func, clock_.getTime() + time, priority, id_++);
			lock_.lock();
			queue_.push(j);
			lock_.unlock();
			return j.id;
		}

		/**
		 * \brief adds new job running repeatedly
		 *
		 * \param[in] interval time after which this job will be scheduled (in microseconds)
		 * \param[in] func function to be called when scheduled
		 * \param[in] priority priority of this job
		 */
		uint64_t runRepeated(uint64_t interval, const boost::function<bool(void)> & func, int16_t priority) {
			// TODO: if first element in queue, adjust waitingtime
			Job j(func, clock_.getTime() + interval, priority, id_++, interval);
			lock_.lock();
			queue_.push(j);
			lock_.unlock();
			return j.id;
		}

		/**
		 * \brief returns current time
		 */
		uint64_t getTime() const {
			return clock_.getTime();
		}

		void stop(uint64_t id) {
			lock_.lock();
			stopMap_[id] = true;
			lock_.unlock();
		}

		/**
		 * \brief Stops whole Scheduler and removes all tasks
		 */
		void Stop() {
			running_ = false;
			lock_.lock();
			while (!queue_.empty()) {
				queue_.pop();
			}
			lock_.unlock();
#ifndef WITH_SIM
			worker_.interrupt();
			worker_.join();
#endif
		}

#ifdef WITH_SIM
		/**
		 * \brief TODO
		 */
		// TODO: convert to a threadpool if needed
		void workerSim() {
			lock_.lock();

			while (!queue_.empty() && queue_.top().time <= clock_.getTime()) {
				Job j = queue_.top();
				queue_.pop();

				bool b = stopMap_.find(j.id) != stopMap_.end();

				if (b) {
					stopMap_.erase(j.id);
				}

				if (b) {
					continue;
				}

				lock_.unlock();

				b = j.func();
				lock_.lock();
				if (b && j.interval != UINT64_MAX) {
					long cTime = clock_.getTime();
					j.time = cTime + j.interval;
					queue_.push(j);
				}
			}
			lock_.unlock();
		}
#endif /* WITH_SIM */

	private:
		Scheduler(const Scheduler &) = delete;
		Scheduler & operator=(const Scheduler &) = delete;

		volatile bool running_;

		util::Clock<ClockUpdater> & clock_;

		std::priority_queue<Job> queue_;

		boost::mutex lock_;

		boost::thread worker_;

		std::atomic<uint64_t> id_;

		std::map<uint64_t, bool> stopMap_;

		// TODO: (Michael) convert to a threadpool if needed
		void worker() {
			uint64_t tID = clock_.registerTimer();
			while (running_) {
				lock_.lock();

				while (running_ && !queue_.empty() && queue_.top().time <= clock_.getTime()) {
					Job j = queue_.top();
					queue_.pop();
					bool b = stopMap_.find(j.id) != stopMap_.end();

					if (b) {
						stopMap_.erase(j.id);
					}

					if (b) {
						continue;
					}

					lock_.unlock();

					b = j.func();

					lock_.lock();
					if (b && j.interval != UINT64_MAX) {
						uint64_t cTime = clock_.getTime();
						j.time = cTime + j.interval;
						queue_.push(j);
					}
				}
				uint64_t t = clock_.getTime() + 1000000; // sleep 1 second if no task is there
				if (!queue_.empty()) {
					t = queue_.top().time;
				}
				lock_.unlock();
				if (!clock_.waitForTime(tID, t)) {
					break;
				}
			}
		}
	};

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_SCHEDULER_H__ */

/**
 * @}
 */
