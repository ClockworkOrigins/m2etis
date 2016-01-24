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
 * \addtogroup simulator
 * @{
 */

#ifndef __M2ETIS_SIM_OMNETCALLBACKINTERFACE_H__
#define __M2ETIS_SIM_OMNETCALLBACKINTERFACE_H__

#include "m2etis/config/GeneratedEventTypes.h"

#include "m2etis/message/M2Message.h"

#include "m2etis/net/NetworkType.h"
#include "m2etis/net/NodeHandle.h"
#include "m2etis/pubsub/config/ChannelName.h"

#include "m2etis/sim/OmNetKeyProvider.h"

namespace m2etis {
namespace sim {

	class OmNetCallbackInterface {
	public:
		/**
		 * \brief destructor
		 */
		virtual ~OmNetCallbackInterface() {}

		/**
		 * \brief send method to be called by OmNetMediator
		 */
		virtual void send(const message::ActionType type, const std::string & to, const message::M2Message<SimulationEventType>::Ptr payload, net::NodeHandle<net::NetworkType<net::OMNET>>::Ptr_const hint, unsigned int size) = 0;

		/**
		 * \brief returns SHA1 key for the given channel
		 */
		virtual std::string getSHA1(int channel_name) = 0;

		/**
		 * brief returns the current time of the simulation
		 */
		virtual long getSimTime() const = 0;
	};

} /* namespace sim */
} /* namespace m2etis */

#endif /* __M2ETIS_SIM_OMNETCALLBACKINTERFACE_H__ */

/**
 *  @}
 */
