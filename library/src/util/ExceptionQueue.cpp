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
#include "m2etis/util/ExceptionQueue.h"

#include "boost/thread.hpp"

namespace m2etis {
namespace util {

	std::queue<loginfo> ExceptionQueue::queue;
	std::mutex ExceptionQueue::mutex;

	void ExceptionQueue::enqueue(const loginfo & data) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(data);
	}

	bool ExceptionQueue::isEmpty() {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	}

	loginfo ExceptionQueue::dequeue() {
		std::lock_guard<std::mutex> lock(mutex);
		loginfo e = queue.front();
		queue.pop();
		return e;
	}

} /* namespace util */
} /* namespace m2etis */
