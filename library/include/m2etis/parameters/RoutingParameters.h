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

//
//  RoutingParameters.h
//  m2etis
//
//  Created by Thomas Fischer on 06.03.13.
//
//

#ifndef m2etis_RoutingParameters_h
#define m2etis_RoutingParameters_h

namespace m2etis {
namespace pubsub {
namespace routing {
	namespace direct {
		const static uint64_t RESUBSCRIPTION_INTERVAL = 5000000;
		const static uint64_t PURGE_DISTANCE = 6000000;
	}
	namespace directbroadcast {
		const static uint64_t RESUBSCRIPTION_INTERVAL = 5000000;
		const static uint64_t PURGE_DISTANCE = 6000000;
	}
	namespace scribe {
		const static uint64_t RESUBSCRIPTION_INTERVAL = 5000000;
		const static uint64_t PURGE_DISTANCE = 6000000;
	}
	namespace spreadit {
		const static uint64_t RESUBSCRIPTION_INTERVAL = 5000000;
		const static uint64_t PURGE_DISTANCE = 6000000;
		const static uint32_t ALLOWED_CHILDS = 2;
	}
	namespace hierarchicalspreadit {
		const static uint64_t RESUBSCRIPTION_INTERVAL = 5000000;
		const static uint64_t PURGE_DISTANCE = 6000000;
	}
}
}
}


#endif
