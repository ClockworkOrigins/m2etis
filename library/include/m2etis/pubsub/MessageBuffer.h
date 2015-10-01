/**
 Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)

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

#ifndef __M2ETIS_PUBSUB_MESSAGEBUFFER_H__
#define __M2ETIS_PUBSUB_MESSAGEBUFFER_H__

#include <map>

#include "m2etis/pubsub/MsgProcess.h"

namespace m2etis {
namespace pubsub {

	class MessageBuffer {
	public:
		MessageBuffer() : buffer_(), counter(0) {
		}

		uint64_t insert(const boost::function<void(void)> & func) {
			buffer_.insert(std::make_pair(counter, func));
			return counter++;
		}

		void deliver(uint64_t id, msgProcess proc) {
			assert(buffer_.find(id) != buffer_.end());
			boost::function<void(void)> p = buffer_[id];
			if (proc == msgProcess::MSG_DELETE || proc == msgProcess::MSG_PROCESS_AND_DELETE) {
				buffer_.erase(id);
				assert(buffer_.find(id) == buffer_.end());
			}
			if (proc == msgProcess::MSG_PROCESS || proc == msgProcess::MSG_PROCESS_AND_DELETE) {
				p();
			}
		}

	private:
		std::map<uint64_t, boost::function<void(void)>> buffer_;
		uint64_t counter;
	};

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_MESSAGEBUFFER_H__ */
