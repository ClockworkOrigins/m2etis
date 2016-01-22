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

#ifndef __M2ETIS_EVENTTRAITS_H__
#define __M2ETIS_EVENTTRAITS_H__

#include "m2etis/pubsub/filter/events/Position.h"

namespace m2etis {

	template <class EventType> struct isSerializable {
		static const int serializable = 0;
	};

	// specializations for EventTypes with serialize function:
	template <> struct isSerializable<pubsub::filter::Position> {
		static const int serializable = 1;
	};

	// specializations for EventTypes with special overloaded transformation functions in PubSubMessage:
	template <> struct isSerializable<std::vector<unsigned char> > {
		static const int serializable = 2;
	};

} /* namespace m2etis */

#endif  /* __M2ETIS_EVENTTRAITS_H__ */

/**
 *  @}
 */
