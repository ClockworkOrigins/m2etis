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

#ifndef __DCB_H__
#define __DCB_H__

#define _GLIBCXX_USE_NANOSLEEP // needed for sleep_for, see http://stackoverflow.com/questions/4438084/stdthis-threadsleep-for-and-gcc

#include <thread>

#include "m2etis/pubsub/DeliverCallbackInterface.h"

class DCB : public m2etis::pubsub::BasicDeliverCallbackInterface<std::vector<unsigned char>> {
public:
	DCB() : _counter(0), _amount(), _timeOut() {
	}
	void deliverCallback(CharVectorEventType m) {
		_counter++;
	}

	void waitUntil(unsigned int messageAmount, uint64_t timeout) {
		_amount = messageAmount;
		_timeOut = timeout;
		std::thread([this]() {
			auto startTime = std::chrono::high_resolution_clock::now();
			do {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			} while (_counter < _amount && _timeOut > uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count()));
		}).join();
	}

	unsigned int _counter;
	unsigned int _amount;
	uint64_t _timeOut;
};

#endif /* __DCB_H__ */
