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
 * @ {
 */

#ifndef __M2ETIS_WRAPPER_UDP_UDPWRAPPER_H__
#define __M2ETIS_WRAPPER_UDP_UDPWRAPPER_H__

#include "boost/asio.hpp"

#include <deque>

#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"
#include "m2etis/net/NetworkType.h"
#include "m2etis/net/NodeHandle.h"

#include "boost/make_shared.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace wrapper {
namespace udp {
	
	/**
	 * \brief creates connections using boosts UdpSocket
	 */
    class UdpWrapper : public net::NetworkInterface<net::NetworkType<net::UDP>> {
	public:
		UdpWrapper(const std::string & ownIP, uint16_t listenPort, const std::string & hostIP, uint16_t hostPort);

		~UdpWrapper();

		/**
		 * \brief calls the other send method
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::UDP>>::Ptr_const hint);

		/**
		 * \brief sends message
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg);

		/**
		 * \brief returns pointer with own data
		 */
		net::NodeHandle<net::NetworkType<net::UDP>>::Ptr getSelfNodeHandle() const {
			if (!_initialized) {
				throw std::runtime_error("UDPWrapper: not initialized. call init first!");
			}

			net::NodeHandle<net::NetworkType<net::UDP>>::Ptr node = boost::make_shared<net::NodeHandle<net::NetworkType<net::UDP>>>();
			std::stringstream os;
			// M2ETIS_LOG_DEBUG("TCPWrapper::getSelfNodeHandle", "Address: " + ownIP)
			os << _name << ":" << _listenPort;
			net::NetworkType<net::UDP>::Key key(os.str());
			node->key_ = key;
			node->hostname_ = _name;
			node->port_ = _listenPort;

			return node;
		}

		/**
		 * \brief Not used right now
		 */
		void registerMessageType(const message::MessageType type, const bool ack) const;

		/**
		 * \brief returns root node
		 */
		net::NetworkType<net::UDP>::Key getRoot() const {
			return _root;
		}

	private:
		typedef std::pair<std::vector<unsigned char>, message::Key<message::IPv4KeyProvider>> msgPair;

		bool _initialized;
		std::string _name;
		std::string _hostName;
		uint16_t _listenPort;
		uint16_t _hostPort;
		boost::asio::io_service _io_service;
		boost::asio::ip::udp::socket * _socket;
		net::NetworkType<net::UDP>::Key _root;

		std::vector<boost::thread *> threads_;
		boost::array<uint8_t, 1048576> recv_buf;

		boost::asio::io_service::strand _strand__;
		// buffers messages till they got sent
		std::deque<msgPair> _outbox;
		boost::asio::io_service::work _work; // keeps the io_service running

		boost::asio::ip::udp::endpoint * _endpoint;
		boost::asio::ip::udp::endpoint * _remote_endpoint;

		void write(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key);

		void writeImpl(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key);

		void write();

		void workerFunc();

		void handleReceive(boost::asio::ip::udp::socket * socket, std::string message, boost::asio::ip::udp::endpoint * endpoint, size_t len);
		void handleReceive(const boost::system::error_code & error, size_t len, boost::asio::ip::udp::endpoint * re);

		UdpWrapper(const UdpWrapper &) = delete;
		UdpWrapper & operator=(const UdpWrapper & rhs) = delete;
	};

} /* namespace udp */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_UDP_UDPWRAPPER_H__ */

/**
 * @}
 */
