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

#ifndef __M2ETIS_PUBSUB_CHANNELTYPEDEFINITIONS_H__
#define __M2ETIS_PUBSUB_CHANNELTYPEDEFINITIONS_H__

#include "m2etis/pubsub/Channel.h"
#include "m2etis/pubsub/config/Strategies.h"

// FIXME refactor to remove using namespace directives
using namespace m2etis::pubsub::routing;
using namespace m2etis::pubsub::filter;
using namespace m2etis::pubsub::persistence;
using namespace m2etis::pubsub::validity;
using namespace m2etis::pubsub::security;
using namespace m2etis::pubsub::partition;
using namespace m2etis::pubsub::order;
using namespace m2etis::pubsub::deliver;
using namespace m2etis::pubsub::rendezvous;

#include "m2etis/pubsub/config/ChannelName.h"

namespace m2etis {
namespace pubsub {

	template <ChannelName channelname>
	struct ChannelT;

	#include "m2etis/config/GeneratedChannelConfiguration.h"

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_CHANNELTYPEDEFINITIONS_H__ */

/**
 *  @}
 */
