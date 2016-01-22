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
 * @ {
 */

#ifndef __M2ETIS_UTILS_EXCEPTIONQUEUE_H__
#define __M2ETIS_UTILS_EXCEPTIONQUEUE_H__

#include <mutex>
#include <queue>

#include "m2etis/util/Exceptions.h"

namespace m2etis {
namespace util {

	/**
	 * \class ExceptionQueue
	 * \brief Implements a queue that supports multiple producers but only one consumer.
	 *
	 * Implements a queue that supports multiple producers but only one consumer by using std::mutex.
	 */
	class ExceptionQueue {
	private:
		static std::queue<loginfo> queue;
		static std::mutex mutex;

	public:
		/**
		 * \brief Enqueues data.
		 *
		 * \param[in] data Data to be queued
		 */
		static void enqueue(const loginfo & data);

		/**
		 * \brief Returns true if the queue is empty.
		 *
		 * \return True if the queue is empty
		 */
		static bool isEmpty();

		/**
		 * \brief Removes the first element in the queue.
		 */
		static loginfo dequeue();
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTILS_EXCEPTIONQUEUE_H__ */

/**
 * @}
 */
