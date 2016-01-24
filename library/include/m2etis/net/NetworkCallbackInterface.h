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
 * \addtogroup net
 * @{
 */

#ifndef __M2ETIS_NET_NETWORKCALLBACKINTERFACE_H__
#define __M2ETIS_NET_NETWORKCALLBACKINTERFACE_H__

#include <stdexcept>

#include "m2etis/message/NetworkMessage.h"
#include "m2etis/net/NodeHandle.h"

namespace m2etis {
namespace net {

	/**
	 * \class NetworkCallbackInterface
	 * \brief Generic interface for the callback-class used for the network
	 *
	 * Implement this class and pass it to KBR_API (or directly to the used wrapper).
	 * The callbacks will be called to inform you about actual network stuff and permit you to
	 * make own decisions.
	 *
	 */
	template<class NetworkType>
	class NetworkCallbackInterface {
	public:
		NetworkCallbackInterface() {}
		virtual ~NetworkCallbackInterface() {}

		/**
		 * \brief called if a message is forwarded by your node
		 *
		 * details you can change the Key, the Message and the next routing hint.
		 * If you want to stop routing simply set hint to 0. Or you can change the key and the message.
		 * Like in scribe, when building a multicast tree. It's like NAT, change the Key and the Message :-)
		 *
		 * \param[in] message the message
		 * \param[in] hint the next routing hop
		 * \return true if the routing must be stopped
		 */
		virtual bool forward(typename message::NetworkMessage<NetworkType>::Ptr message, const typename NodeHandle<NetworkType>::Ptr hint) = 0;

		/**
		 * \brief called if a message is delivered to your node.
		 *
		 * \param[in] message the message
		 */
		virtual void deliver(typename message::NetworkMessage<NetworkType>::Ptr message) = 0;

		/**
		 * \brief called on node joins or leaves
		 *
		 * \param[in] key the node's key
		 * \param[in] handle the node's handle
		 * \param[in] joined true if joined
		 */
		virtual void update(const typename NetworkType::Key & key, const typename NodeHandle<NetworkType>::Ptr_const handle, bool joined) = 0;
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NETWORKCALLBACKINTERFACE_H__ */

/**
 * @}
 */
