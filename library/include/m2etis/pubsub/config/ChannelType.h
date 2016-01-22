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

#ifndef __M2ETIS_PUBSUB_CHANNELTYPE_H__
#define __M2ETIS_PUBSUB_CHANNELTYPE_H__

namespace m2etis {
namespace pubsub {

	template <
		class RoutingT,
		class FilterT,
		class OrderT,
		class DeliverT,
		class PersistenceT,
		class ValidityT,
        class PartitionT,
		class SecurityT,
		class RendezvousT
	>
	class ChannelType {
	public:
		typedef RoutingT RoutingStrategy;
		typedef FilterT FilterStrategy;
		typedef OrderT OrderStrategy;
		typedef DeliverT DeliverStrategy;
		typedef PersistenceT PersistenceStrategy;
		typedef ValidityT ValidityStrategy;
        typedef PartitionT PartitionStrategy;
		typedef SecurityT SecurityStrategy;
		typedef RendezvousT RendezvousStrategy;
	};

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_CHANNELTYPE_H__ */

/**
 *  @}
 */
