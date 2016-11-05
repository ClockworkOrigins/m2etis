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

#ifndef __M2ETIS_NET_NETWORKCONTROLLER_H__
#define __M2ETIS_NET_NETWORKCONTROLLER_H__

#include <condition_variable>
#include <map>

#include "m2etis/util/Logger.h"

#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"

#include "m2etis/parameters/QueueParameters.h"

#include "m2etis/pubsub/ForwardInfo.h"
#include "m2etis/pubsub/PubSubSystemEnvironment.h"

#include "boost/shared_ptr.hpp"

#include "clockUtils/container/DoubleBufferQueue.h"

namespace m2etis {
namespace net {

	/**
	 * \class NetworkController
	 * \brief NetworkController
	 *
	 * Long Desc
	 */
	template<class NetworkType>
	class NetworkController : public NetworkCallbackInterface<NetworkType> {
	public:
		typedef std::function<void(typename message::NetworkMessage<NetworkType>::Ptr message)> net_deliver_func;
		typedef std::function<pubsub::FIPtr(typename message::NetworkMessage<NetworkType>::Ptr message)> net_forward_func;

		// TODO: (Daniel) public access to pointer isn't what we normally do, so refactor to be conform with our normal coding style
		NetworkInterface<NetworkType> * network_;

		/**
		 * \brief creates new interface for communication with a wrapper
		 * adds a polling job for incoming messages
		 */
		NetworkController(NetworkInterface<NetworkType> * network, pubsub::PubSubSystemEnvironment * pssi) : network_(network), _running(true), deliver_map_(), forward_map_(), msgQueue_(), _sendLock(), _sendConditionVariable(), _sendThread(std::bind(&NetworkController::sendWorker, this)), pssi_(pssi) {
			processingID_ = pssi->scheduler_.runRepeated(parameters::PULL_DELIVERQUEUE, std::bind(&NetworkController::processDeliverQueue, this), 3);
			network_->setCallback(this);
		}

		/**
		 * \brief stops polling job and cleans up wrapper
		 */
		~NetworkController() {
			{
				std::unique_lock<std::mutex> ul(_sendLock);
				_running = false;
				pssi_->scheduler_.stop(processingID_);
				_sendConditionVariable.notify_one();
			}
			_sendThread.join();
			delete network_;
			network_ = nullptr;
		}

		/**
		 * \todo
		 */
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

		/**
		 * \brief called from wrapper for every arriving message
		 * because of this method call can be done from different threads,
		 * messages are pushed into a queue being worked on in the m2etis thread
		 */
		void deliver(typename message::NetworkMessage<NetworkType>::Ptr message) {
			if (*message->typePtr == 0) {
				M2ETIS_THROW_FAILURE("NetworkController - deliver", "invalid message type", -1);
			}

			msgQueue_.push(message);
		}

		/**
		 * \todo
		 */
		void update(const typename NetworkType::Key &, const typename NodeHandle<NetworkType>::Ptr_const, bool) {
		}

		/**
		 * \brief sends a message to the receiver defined within the message
		 * if the sender is the receiver, message is put into deliver method directly to remove overhead by serializing and trying to send message
		 * otherwise the message is forwarded to the corresponding wrapper
		 */
		void send(typename message::NetworkMessage<NetworkType>::Ptr msg) {
			// don't send message from me to me over network, can be delivered directly
			if (msg->receiver == msg->sender) {
				deliver(msg);
				return;
			}
			std::unique_lock<std::mutex> ul(_sendLock);
			_sendQueue.push(msg);
			_sendConditionVariable.notify_one();
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

	private:
		bool _running;

		typedef std::map<message::MessageType, net_deliver_func> DMapType;
		DMapType deliver_map_;
		typedef std::map<message::MessageType, net_forward_func> FMapType;
		FMapType forward_map_;
		typedef clockUtils::container::DoubleBufferQueue<typename message::NetworkMessage<NetworkType>::Ptr, true, false> DIQueueType;
		DIQueueType msgQueue_;
		clockUtils::container::DoubleBufferQueue<typename message::NetworkMessage<NetworkType>::Ptr, false, false> _sendQueue;
		std::mutex _sendLock;
		std::condition_variable _sendConditionVariable;
		std::thread _sendThread;

		pubsub::PubSubSystemEnvironment * pssi_;

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
				typename message::NetworkMessage<NetworkType>::Ptr msg;
				clockUtils::ClockError err = msgQueue_.poll(msg);

				if (err == clockUtils::ClockError::SUCCESS) {
					typename DMapType::iterator it = deliver_map_.find(*msg->typePtr);
					if (it != deliver_map_.end()) {
						it->second(msg);
					}
				}
			}
			return _running;
		}

		void sendWorker() {
			do {
				{ // synchronize with destructor. Inside a scope to unlock _condMutex after waiting
					// acquire the condition mutex
					std::unique_lock<std::mutex> ul(_sendLock);
					// The normal case is waiting for new write request
					// Exceptions are:
					//	* !_running  -> socket will be closed soon
					//	* !empty() -> new requests arrived
					// this check needs to be inside the lock to avoid a lost wake-up
					// if this 'if' or the 'else if' is true, the lock ensures that the notify is called *after* the wait call
					if (!_running || !_sendQueue.empty()) {
						// nothing
					} else {
						_sendConditionVariable.wait(ul);
					}
				}
				while (!_sendQueue.empty() && _running) {
					typename message::NetworkMessage<NetworkType>::Ptr msg;
					if (clockUtils::ClockError::SUCCESS == _sendQueue.poll(msg)) {
						network_->send(msg, typename NodeHandle<NetworkType>::Ptr_const());
					}
				}
			} while (_running);
		}

		// make non-copyable
		NetworkController & operator=(const NetworkController & rhs) = delete;

		NetworkController(const NetworkController & rhs) = delete;
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NETWORKCONTROLLER_H__ */

/**
 * @}
 */
