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

#ifndef __M2ETIS_UTIL_DOUBLEBUFFERQUEUE_H__
#define __M2ETIS_UTIL_DOUBLEBUFFERQUEUE_H__

#include <queue>

#include "m2etis/util/Exceptions.h"

#include "boost/thread/recursive_mutex.hpp"

namespace m2etis {
namespace util {

	template<typename T, bool producer = true, bool consumer = true>
	class DoubleBufferQueue {
	public:
		/**
		 * \brief default constructor
		 */
		M2ETIS_DEPRECATED DoubleBufferQueue() : _queueA(), _queueB(), _queueRead(&_queueA), _queueWrite(&_queueB), _lock() {
		}

		/**
		 * \brief pushes the given value into the queue
		 */
		M2ETIS_DEPRECATED void push(const T & value) {
			boost::recursive_mutex::scoped_lock scopeLock(_lock);
			_queueWrite->push(value);
			assert(!empty());
		}

		/**
		 * \brief removes first entry of the queue
		 */
		M2ETIS_DEPRECATED void pop() {
			boost::recursive_mutex::scoped_lock scopeLock(_lock);
			if (_queueRead->empty()) {
				swap();
			}

			if (_queueRead->empty()) {
				M2ETIS_THROW_API("DoubleBufferQueue", "nothing to pop");
			}

			_queueRead->pop();
		}

		/**
		 * \brief returns first entry of the queue
		 */
		M2ETIS_DEPRECATED T front() {
			boost::recursive_mutex::scoped_lock scopeLock(_lock);
			if (_queueRead->empty()) {
				swap();
			}

			if (_queueRead->empty()) {
				M2ETIS_THROW_API("DoubleBufferQueue", "nothing to get");
			}

			return _queueRead->front();
		}

		/**
		 * \brief remoes first entry of the queue and returns its value
		 */
		M2ETIS_DEPRECATED T poll() {
			boost::recursive_mutex::scoped_lock scopeLock(_lock);
			if (_queueRead->empty()) {
				swap();
			}

			if (_queueRead->empty()) {
				M2ETIS_THROW_API("DoubleBufferQueue", "nothing to get");
			}

			T ret = _queueRead->front();
			_queueRead->pop();
			return ret;
		}

		/**
		 * \brief returns true if the queue is empty, otherwise false
		 */
		M2ETIS_DEPRECATED bool empty() const {
			return _queueRead->empty() && _queueWrite->empty();
		}

		/**
		 * \brief returns size of the queue
		 */
		M2ETIS_DEPRECATED size_t size() const {
			return _queueRead->size() + _queueWrite->size();
		}

		/**
		 * \brief removes all elements in the queue
		 */
		M2ETIS_DEPRECATED void clear() {
			boost::recursive_mutex::scoped_lock scopeLock(_lock);
			while(!_queueRead->empty()) {
				_queueRead->pop();
			}
			while(!_queueWrite->empty()) {
				_queueWrite->pop();
			}
		}

	private:
		std::queue<T> _queueA;
		std::queue<T> _queueB;

		std::queue<T> * _queueRead;
		std::queue<T> * _queueWrite;

		boost::recursive_mutex _lock;

		DoubleBufferQueue(const DoubleBufferQueue &);

		/**
		 * \brief switches read and write buffer
		 */
		void swap() {
			boost::recursive_mutex::scoped_lock scopeLock(_lock);
			if (_queueRead == &_queueA) {
				_queueWrite = &_queueA;
				_queueRead = &_queueB;
			} else {
				_queueWrite = &_queueB;
				_queueRead = &_queueA;
			}
		}
	};

} /* namespace util */
} /* namespace m2etis */

#endif /* __M2ETIS_UTIL_DOUBLEBUFFERQUEUE_H__ */

/**
 * @}
 */
