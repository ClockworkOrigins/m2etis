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
 * \addtogroup message
 * @{
 */

#ifndef __M2ETIS_MESSAGE_M2MESSAGE_H__
#define __M2ETIS_MESSAGE_M2MESSAGE_H__

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/message/MessageType.h"

#include "boost/make_shared.hpp"
#include "boost/serialization/export.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"

namespace m2etis {
namespace message {

	template<class EventType>
	class M2Message {
	public:
		// Message Ptr
		typedef boost::shared_ptr<M2Message<EventType> > Ptr;
		// Payload Ptr
		typedef boost::shared_ptr<EventType> PayloadPtr;

	    MessageType * typePtr;

	    /**
		 * \brief payload
		 */
	    PayloadPtr payload;

		M2Message() : typePtr(), payload(boost::make_shared<EventType>()) {
		}

		explicit M2Message(MessageType * mt) : typePtr(mt), payload(boost::make_shared<EventType>()) {
		}

		M2Message(const EventType & v, MessageType * mt) : typePtr(mt), payload(boost::make_shared<EventType>(v)) {
		}

		M2Message(const M2Message & msg, MessageType * mt) : typePtr(mt), payload(boost::make_shared<EventType>(*msg.payload)) {
		}

		virtual ~M2Message() {}

		// TODO: (Michael) remove. won't work for most EventTypes
		// TODO: (Daniel) currently used by one of the testcases, needs to be rebuilt there
		inline std::string getPayloadString() {
			return std::string(payload->begin(), payload->end());
		}

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
#ifndef WITH_SIM
			ar & payload; // Don't serialize payload for simulator, payload size is set in config
#endif /* WITH_SIM */
		}
	};

	typedef M2Message<std::vector<unsigned char>> M2etisMessage;
	std::ostream & operator<<(std::ostream & s, const M2etisMessage::Ptr);

#ifdef WITH_SIM
	typedef M2Message<SimulationEventType> M2SimMessage;
	std::ostream & operator<<(std::ostream & s, const M2SimMessage::Ptr);
#endif /* WITH_SIM */

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_M2MESSAGE_H__ */

/**
 *  @}
 */
