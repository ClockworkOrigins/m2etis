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

#ifndef __M2ETIS_NET_NETWORKCONTROLLER_H__
#define __M2ETIS_NET_NETWORKCONTROLLER_H__

#include <map>

#include "m2etis/util/DoubleBufferQueue.h"
#include "m2etis/util/Logger.h"

#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"

#include "m2etis/parameters/QueueParameters.h"

#include "m2etis/pubsub/ForwardInfo.h"
#include "m2etis/pubsub/PubSubSystemEnvironment.h"

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace net {

	/**
	 * \ingroup net
	 *
	 * \class NetworkController
	 * \brief NetworkController
	 *
	 * Long Desc
	 *
	 */
	template<class NetworkType>
	class NetworkController : public NetworkCallbackInterface<NetworkType> {
	public:
		typedef boost::function<void(typename message::NetworkMessage<NetworkType>::Ptr message)> net_deliver_func;
		typedef boost::function<pubsub::FIPtr(typename message::NetworkMessage<NetworkType>::Ptr message)> net_forward_func;

	private:
		// Subject to refactor:
		struct DeliverInfo {
			explicit DeliverInfo(typename message::NetworkMessage<NetworkType>::Ptr ms) : msg_type(*ms->typePtr), message(ms) {}
			message::MessageType msg_type;
			typename message::NetworkMessage<NetworkType>::Ptr message;
		};

		typedef std::map<message::MessageType, net_deliver_func> DMapType;
		DMapType deliver_map_;
		typedef std::map<message::MessageType, net_forward_func> FMapType;
		FMapType forward_map_;
		typedef util::DoubleBufferQueue<DeliverInfo> DIQueueType;
		DIQueueType msgQueue_;

		pubsub::PubSubSystemEnvironment * pssi_;

		bool _running;

		uint64_t processingID_;

		bool processDeliverQueue() {
			/* Was will ich hier:
			 * Später darf sich ein Threadpool um die Einträge in der Queue kümmern.
			 * Beim Rausnehmen sollen die sich natürlich nicht in die Quere kommen.
			 */
			if (!_running) {
				return false;
			}
			while (!msgQueue_.empty()) {
				DeliverInfo di = msgQueue_.poll();

				typename DMapType::iterator it = deliver_map_.find(di.msg_type);

				if (it != deliver_map_.end()) {
					it->second(di.message);
				}
			}
			return true;
		}

		// make non-copyable

		NetworkController & operator=(const NetworkController & rhs) = delete;

		NetworkController(const NetworkController & rhs) = delete;

	public:
		NetworkInterface<NetworkType> * network_;

		NetworkController(NetworkInterface<NetworkType> * network, pubsub::PubSubSystemEnvironment * pssi) :
			deliver_map_(),
			forward_map_(),
			msgQueue_(),
			pssi_(pssi), _running(true), network_(network) {
			processingID_ = pssi->scheduler_.runRepeated(parameters::PULL_DELIVERQUEUE, boost::bind(&NetworkController::processDeliverQueue, this), 3);
			network_->setCallback(this);
		}

		virtual ~NetworkController() {
			pssi_->scheduler_.stop(processingID_);
			_running = false;
			delete network_;
			network_ = nullptr;
		}

		// methods inherited from NetworkCallback
		bool forward(typename message::NetworkMessage<NetworkType>::Ptr message, const typename NodeHandle<NetworkType>::Ptr hint) {
			typename FMapType::const_iterator iter = forward_map_.find(*message->typePtr);
			if (iter != forward_map_.end()) {
				pubsub::FIPtr ptr = iter->second(message);
				if (ptr->stop) {
					// FIXME STOP THE MESSAGE
					return true;
				}
			}
			return false;
		}

		void deliver(typename message::NetworkMessage<NetworkType>::Ptr message) {
			if (*message->typePtr == 0) {
				M2ETIS_THROW_FAILURE("NetworkController - deliver", "invalid message type", -1);
			}

			msgQueue_.push(DeliverInfo(message));
		}

		void update(const typename NetworkType::Key &, const typename NodeHandle<NetworkType>::Ptr_const, bool) {}

		// methods provided for Topics
		void send(typename message::NetworkMessage<NetworkType>::Ptr msg) {
			// don't send message from me to me over network, can be delivered directly
			if (msg->receiver == msg->sender) {
				deliver(msg);
				return;
			}
			network_->send(msg, typename NodeHandle<NetworkType>::Ptr_const());
		}

		void registerMessageType(message::MessageType type, const bool ack = true) {
			network_->registerMessageType(type, ack);
		}

		void register_forward(message::MessageType nr, net_forward_func f) {
			forward_map_.insert(std::make_pair(nr, f));
		}

		void register_deliver(message::MessageType nr, net_deliver_func f) {
			deliver_map_.insert(std::make_pair(nr, f));
		}

		void deregister_forward(message::MessageType nr) {
			forward_map_.erase(nr);
		}

		void deregister_deliver(message::MessageType nr) {
			deliver_map_.erase(nr);
		}

		inline typename NetworkType::Key getSelf() const {
			return network_->getSelfNodeHandle()->key_;
		}
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NETWORKCONTROLLER_H__ */

/**
 *  @}
 */
