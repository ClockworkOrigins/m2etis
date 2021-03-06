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
 * \addtogroup pubsub
 * @{
 */

#ifndef __M2ETIS_PUBSUB_RENDEZVOUS_BASERENDEZVOUS_H__
#define __M2ETIS_PUBSUB_RENDEZVOUS_BASERENDEZVOUS_H__

#include "m2etis/pubsub/rendezvous/BaseRendezvous.h"

namespace m2etis {
namespace pubsub {
namespace rendezvous {

	class BaseRendezvous {
	public:
		explicit BaseRendezvous(const std::vector<std::string> & rootList) : _rootList(rootList) {
			assert(!_rootList.empty());
		}

		virtual ~BaseRendezvous() {
			_rootList.clear();
		}

		virtual std::string getRoot() = 0;

	protected:
		std::vector<std::string> _rootList;
	};

} /* namespace rendezvous */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_RENDEZVOUS_BASERENDEZVOUS_H__ */

/**
 * @}
 */
