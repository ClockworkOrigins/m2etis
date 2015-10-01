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

#ifndef __M2ETIS_MESSAGE_NETWORKMESSAGE_H__
#define __M2ETIS_MESSAGE_NETWORKMESSAGE_H__

#include "m2etis/config/GeneratedEventTypes.h"

#include "m2etis/message/key/providers/KeyProviders.h"

#include "m2etis/net/NetworkType.h"

#include "boost/serialization/export.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/variant.hpp"
#include "boost/serialization/vector.hpp"

namespace m2etis {
namespace message {

	template<class NetworkType>
	class NetworkMessage {
	public:
		// Message Ptr
		typedef boost::shared_ptr<NetworkMessage> Ptr;

        /**
		 * \brief sender of the message
		 */
		typename NetworkType::Key sender;

		/**
		 * \brief receiver of the message
		 */
		typename NetworkType::Key receiver;

		MessageType * typePtr;

		NetworkMessage() : sender(), receiver(), typePtr() {
		}

		explicit NetworkMessage(MessageType * mt) : sender(), receiver(), typePtr(mt) {
		}

		NetworkMessage(const NetworkMessage & msg, MessageType * mt) : sender(msg.sender), receiver(msg.receiver), typePtr(mt) {
		}

		virtual ~NetworkMessage() {}

#ifdef WITH_SIM
		SimulationEventType pl;
#endif /* WITH_SIM */

	private:
		friend class boost::serialization::access;
		template <typename Archive> void serialize(Archive & ar, const unsigned int /*version*/) {
	        ar & sender;
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_NETWORKMESSAGE_H__ */
