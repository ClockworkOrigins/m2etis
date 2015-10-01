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

/**
 * \addtogroup net
 * @ {
 */

#ifndef __M2ETIS_NET_NETWORKINTERFACE_H__
#define __M2ETIS_NET_NETWORKINTERFACE_H__

#include "m2etis/util/SystemParameters.h"

#include "m2etis/message/MessageType.h"
#include "m2etis/message/NetworkMessage.h"
#include "m2etis/net/NodeHandle.h"

namespace m2etis {
namespace net {

	template<class Network>
	class NetworkCallbackInterface;

	/**
	 * \ingroup net
	 *
	 * \class NetworkInterface
	 * \brief NetworkInterface
	 *
	 * Long Desc
	 *
	 */
	template<class NetworkType>
	class NetworkInterface {
	public:
		virtual ~NetworkInterface() {
		}

		/**
		 * \brief Sets the callback-object on which the defined callbacks will be called
		 * \param[in] cb The callback pointer
		 */
		void setCallback(NetworkCallbackInterface<NetworkType> * cb) {
			_callback = cb;
		}

		/**
		 * \brief Sends a message to the node which is responsible for the given key. Provide a hint for better routing.
		 * \param[in] msg The Message to send
		 * \param[in] hint The additional hint for routing, may be null
		 */
		virtual void send(const typename message::NetworkMessage<NetworkType>::Ptr msg, typename NodeHandle<NetworkType>::Ptr_const hint) = 0;

		/**
		 * \brief query your NodeHandle. You may query that for it's key.
		 * \return the NodeHandle to your underlying node.
		 */
		virtual typename NodeHandle<NetworkType>::Ptr getSelfNodeHandle() const = 0;

		/**
		 * \brief register a messagetype. Only registered types are sent.
		 * \param[in] type a number > 10
		 * \param[in] ack true if messages with this type should be acknowleged in network layer
		 */
		virtual void registerMessageType(const typename NetworkType::MessageType type, const bool ack) const = 0;

		// virtual typename NetworkType::Message::Ptr createNetMessage() = 0;

		// virtual typename NetworkType::Message::Ptr deserializeMessage(const std::vector<unsigned char>& serialized_message) = 0;

		/**
		 * @brief returns an array of count neighbor nodes with priority to closer nodes.
		 * @param num the count
		 */
		// virtual NodeHandleList neighborSet(const int num) const = 0;

		// virtual NodeHandleList replicaSet(const std::string& key, const int maxRank) const  = 0;

		// virtual bool range(const NodeHandle& node, const int rank, const std::string& lkey, const std::string& rkey) const = 0;

	protected:
		net::NetworkCallbackInterface<NetworkType> * _callback;
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NETWORKINTERFACE_H__ */

/** @}
 */
