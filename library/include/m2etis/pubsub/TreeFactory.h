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

#ifndef __M2ETIS_PUBSUB_TREEFACTORY_H__
#define __M2ETIS_PUBSUB_TREEFACTORY_H__

#include "m2etis/pubsub/Tree.h"
#include "m2etis/pubsub/config/ChannelName.h"

namespace m2etis {
namespace pubsub {

	/**
	 * \ingroup pubsub
	 *
	 * \class TreeFactory
	 * \brief TreeFactory builds trees with a unique MessageType.
	 *
	 * There may only be one TreeFactory per PubSubSystem, as the MessageTypes must be unique througout one m2etis instance. Only for usage in the context class.
	 * In order to work correctly, the instanciation sequence of trees may not vary during initialization of m2etis.
	 */
	class TreeFactory {
		// This variable holds the next tree id, for usage as message type
		uint16_t next_treename = CHANNEL_COUNT;

	public:
		template<class ChannelType, class NetworkType, class EventType>
		Tree<ChannelType, NetworkType, EventType> * createTree(const typename NetworkType::Key & self, const typename NetworkType::Key & rendezvous, const typename NetworkType::Key & root, PubSubSystemEnvironment * pssi, int cI) {
			auto * ret = new Tree<ChannelType, NetworkType, EventType>(next_treename, self, rendezvous, root, pssi, cI);

			if (ret != nullptr) {
				++next_treename;
			}

			return ret;
		}
	};

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_TREEFACTORY_H__ */

/**
 *  @}
 */
