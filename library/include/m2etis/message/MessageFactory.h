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

#ifndef __M2ETIS_MESSAGE_MESSAGEFACTORY_H__
#define __M2ETIS_MESSAGE_MESSAGEFACTORY_H__

#include "m2etis/message/InternalMessage.h"

namespace m2etis {
namespace message {

	/**
	 * \class MessageFactory
	 * \brief Message Factory to create messages
	 *
	 * Long Desc
	 *
	 */
	template<class ChannelType, class NetworkType>
	class MessageFactory {
	public:
		/**
		 * \brief creates a Message
		 *
		 * \param[in] w requested message
		 *
		 * \return Returns an object of the message
		 */
	    template<class EventType>
		typename InternalMessage<NetworkType, ChannelType, EventType>::Ptr createMessage(const EventType & w) const {
			return boost::make_shared<InternalMessage<NetworkType, ChannelType, EventType>>(w);
		}

	    template<class EventType>
		typename InternalMessage<NetworkType, ChannelType, EventType>::Ptr createMessage() const {
			return boost::make_shared<InternalMessage<NetworkType, ChannelType, EventType>>();
		}
	};

} /* namespace message */
} /* namespace m2etis */

#endif /* __M2ETIS_MESSAGE_MESSAGEFACTORY_H__ */

/**
 * @}
 */
