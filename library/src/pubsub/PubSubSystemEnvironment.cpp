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

#ifdef ANDROID
	#define __STDC_LIMIT_MACROS
#endif

#include "m2etis/pubsub/PubSubSystemEnvironment.h"

#include "m2etis/net/NetworkFactory.h"

#include "m2etis/pubsub/TreeFactory.h"

namespace m2etis {
namespace pubsub {

	PubSubSystemEnvironment::PubSubSystemEnvironment(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort) : clock_(), scheduler_(clock_), _factory(new net::NetworkFactory(listenIP, listenPort, connectIP, connectPort, this)), _tree_factory(new TreeFactory()) {
	}

	PubSubSystemEnvironment::~PubSubSystemEnvironment() {
		delete _factory;
		delete _tree_factory;
	}

} /* namespace pubsub */
} /* namespace m2etis */
