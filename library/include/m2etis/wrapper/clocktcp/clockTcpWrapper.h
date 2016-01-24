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
 * \addtogroup wrapper
 * @{
 */

#ifndef __M2ETIS_WRAPPER_CLOCKTCPWRAPPER_H__
#define __M2ETIS_WRAPPER_CLOCKTCPWRAPPER_H__

#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"
#include "m2etis/net/NetworkType.h"
#include "m2etis/net/NodeHandle.h"

#include "boost/make_shared.hpp"
#include "boost/thread/mutex.hpp"

namespace boost {
	class thread;
} /* namespace boost */
namespace clockUtils {
namespace sockets {
	class TcpSocket;
} /* namespace sockets */
} /* namespace clockUtils */

namespace m2etis {
namespace wrapper {
namespace clocktcp {

	/**
	 * \brief creates connections using clockUtils' TcpSocket
	 */
	class clockTcpWrapper : public net::NetworkInterface<net::NetworkType<net::clockTCP>> {
	public:
		/**
		 * \brief starts a TcpSocket connection listening
		 */
		clockTcpWrapper(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort);

		/**
		 * \brief closes socket and all its connections
		 */
		~clockTcpWrapper();

		/**
		 * \brief sends a message
		 * \param[in] msg Message to be sent
		 * \param[in] hint A hint on the next hop to be used (not used in this wrapper)
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::clockTCP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::clockTCP>>::Ptr_const hint);

		/**
		 * \brief returns pointer with data
		 * \return data contains _name, _port and key from (_name:_port)
		 */
		net::NodeHandle<net::NetworkType<net::clockTCP>>::Ptr getSelfNodeHandle() const {
			if (!_initialized) {
				throw std::runtime_error("clockTCPWrapper: not initialized. call init() first!");
			}

			net::NodeHandle<net::NetworkType<net::clockTCP>>::Ptr node = boost::make_shared<net::NodeHandle<net::NetworkType<net::clockTCP>>>();
			node->key_ = _local;
			node->hostname_ = _local.ipStr();
			node->port_ = _local.getPort();

			return node;
		}

		/**
		 * \brief Doesn't do anything right now
		 */
		void registerMessageType(const message::MessageType type, const bool ack) const;

	private:
		clockTcpWrapper(const clockTcpWrapper &) = delete;
		clockTcpWrapper & operator=(const clockTcpWrapper & rhs) = delete;

		net::NetworkType<net::clockTCP>::Key real2metis(net::NetworkType<net::clockTCP>::Key key);

		net::NetworkType<net::clockTCP>::Key metis2real(net::NetworkType<net::clockTCP>::Key key);

		void eraseSocket(net::NetworkType<net::clockTCP>::Key realKey);

		bool _initialized;

		net::NetworkType<net::clockTCP>::Key _local;

		boost::mutex _lock; // used to lock _sockets
		std::map<net::NetworkType<net::clockTCP>::Key, clockUtils::sockets::TcpSocket *> _sockets;

		/**
		 * \brief called after incoming connection. New thread
		 */
		void readFromSocket(clockUtils::sockets::TcpSocket * oldSocket);


		std::map<net::NetworkType<net::clockTCP>::Key, net::NetworkType<net::clockTCP>::Key> _mapping_metis_real;
		std::map<net::NetworkType<net::clockTCP>::Key, net::NetworkType<net::clockTCP>::Key> _mapping_real_metis;
		boost::mutex _mapLock;

		/**
		 * \brief stores all threads created in this class for cleanup issues
		 */
		std::multimap<uint32_t, boost::thread *> _threads;
	};

} /* namespace clocktcp */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_CLOCKTCPWRAPPER_H__ */

/**
 * @}
 */
