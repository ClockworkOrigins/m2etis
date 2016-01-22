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

#ifndef __M2ETIS_PUBSUB_CHANNELINTERFACE_H__
#define __M2ETIS_PUBSUB_CHANNELINTERFACE_H__

#include "m2etis/message/M2Message.h"
#include "m2etis/message/NetworkMessage.h"
#include "m2etis/net/NetworkType.h"
#include "m2etis/pubsub/config/ChannelName.h"

namespace m2etis {
namespace pubsub {
namespace filter {
	template <typename EventType> class FilterExp;
} /* namespace filter */

	template<class T>
	class BasicDeliverCallbackInterface;

	/**
	 * \ingroup pubsub
	 *
	 * \class ChannelInterface
	 * \brief ChannelInterface
	 *
	 * Long Desc
	 *
	 */
	template<class EventType>
	struct BasicChannelInterface {
		/**
		 * \brief Destructor
		 */
		virtual ~BasicChannelInterface() {}

		/**
		 * \brief publishes a message on the channel
		 *
		 * \param[in] msg published message on the channel
		 */
		virtual void publish(const typename message::M2Message<EventType>::Ptr msg) = 0;

		/**
		 * \brief subscribes to the channel
		 *
		 * \param[in] callback callback method
		 * \param[in] predicate filter
		 */
		virtual void subscribe(BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> >) = 0;

		virtual void unsubscribe() = 0;

		/**
		 * \brief deregisters single filter
		 *
		 * \param[in] predicate filter
		 */
		virtual void unsubscribe(boost::shared_ptr<filter::FilterExp<EventType> >) = 0;

		/**
		 * \brief Gets a identification of the channel
		 *
		 * \return Returns an enum identifying the channel
		 */
		virtual ChannelName getChannel() const = 0;

		virtual std::string getSelf() const = 0;

		virtual typename message::M2Message<EventType>::Ptr createMessage() const = 0;

		virtual typename message::M2Message<EventType>::Ptr createMessage(const EventType & payload) const = 0;

#ifdef WITH_SIM
		virtual typename message::NetworkMessage<net::NetworkType<net::OMNET> >::Ptr transformToNetworkMessage(typename message::M2Message<EventType>::Ptr msg) const = 0;

		virtual typename message::M2Message<EventType>::Ptr transformToM2Message(typename message::NetworkMessage<net::NetworkType<net::OMNET> >::Ptr msg) const = 0;
#endif /* WITH_SIM */
	};

	typedef BasicChannelInterface<std::vector<unsigned char> > ChannelInterface;

} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_CHANNELINTERFACE_H__ */

/**
 *  @}
 */
