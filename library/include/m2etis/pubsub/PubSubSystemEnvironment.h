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
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_PUBSUBSYSTEMENVIRONMENT_H__
#define __M2ETIS_PUBSUB_PUBSUBSYSTEMENVIRONMENT_H__

#include <string>

#include "m2etis/pubsub/Scheduler.h"

#ifndef WITH_SIM
	#include "m2etis/util/RealTimeClock.h"
#else
	#include "m2etis/sim/OmNetClock.h"
#endif

#include "m2etis/util/Clock.h"

namespace m2etis {
namespace net {
	class NetworkFactory;
} /* namespace net */

namespace pubsub {

	class TreeFactory;

	class PubSubSystemEnvironment {
	public:
		PubSubSystemEnvironment(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort);

		~PubSubSystemEnvironment();

#ifndef WITH_SIM
		util::Clock<util::RealTimeClock> clock_;
		Scheduler<util::RealTimeClock> scheduler_;
#else
		util::Clock<sim::OmNetClock> clock_;
		Scheduler<sim::OmNetClock> scheduler_;
#endif
		net::NetworkFactory * _factory;
		TreeFactory * _tree_factory;

	private:
		PubSubSystemEnvironment();
		void clockUpdater();
	};

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_PUBSUBSYSTEMENVIRONMENT_H__ */

/**
 *  @}
 */
