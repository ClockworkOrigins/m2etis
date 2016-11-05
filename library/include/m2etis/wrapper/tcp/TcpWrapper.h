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

#ifndef __M2ETIS_WRAPPER_TCPWRAPPER_H__
#define __M2ETIS_WRAPPER_TCPWRAPPER_H__

#include "boost/asio.hpp"

#include <deque>
#include <mutex>
#include <set>

#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"
#include "m2etis/net/NetworkType.h"
#include "m2etis/net/NodeHandle.h"

#include "boost/make_shared.hpp"

namespace std {
	class thread;
} /* namespace std */

namespace m2etis {
namespace wrapper {
namespace tcp {
	
	/**
	 * \brief creates connections using boosts TcpSocket
	 */
	class TcpWrapper : public net::NetworkInterface<net::NetworkType<net::TCP>> {
	public:
		TcpWrapper(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort);

		~TcpWrapper();

		/**
		 * \brief calls the other send method
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::TCP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::TCP>>::Ptr_const hint);

		/**
		 * \brief sends message
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::TCP>>::Ptr msg);

		/**
		 * \brief returns pointer with own data
		 */
		net::NodeHandle<net::NetworkType<net::TCP>>::Ptr getSelfNodeHandle() const {
			if (!_initialized) {
				throw std::runtime_error("TCPWrapper: not initialized. call init first!");
			}

			net::NodeHandle<net::NetworkType<net::TCP>>::Ptr node = boost::make_shared<net::NodeHandle<net::NetworkType<net::TCP>>>();
			node->key_ = _local;
			node->hostname_ = _local.ipStr();
			node->port_ = _local.getPort();

			return node;
		}

		/**
		 * \brief Not used right now
		 */
		void registerMessageType(const message::MessageType type, const bool ack) const;
		
		/**
		 * \brief returns root node
		 */
		net::NetworkType<net::TCP>::Key getRoot() const {
			return _rendezvouz;
		}

	private:
		typedef std::pair<std::vector<uint8_t>, boost::asio::ip::tcp::socket *> msgPair;

		bool _initialized;

		net::NetworkType<net::TCP>::Key _local;
		net::NetworkType<net::TCP>::Key _rendezvouz;

		boost::asio::io_service _io_service;
		boost::asio::ip::tcp::acceptor * _acceptor;

		std::mutex lock_; // used to lock _sockets
		std::map<net::NetworkType<net::TCP>::Key, boost::asio::ip::tcp::socket *> _sockets;

		boost::asio::io_service::strand _strand__;

		// buffers messages till they got sent
		std::deque<msgPair> _outbox;
		boost::asio::io_service::work * _work; // keeps the io_service running

		std::map<net::NetworkType<net::TCP>::Key, net::NetworkType<net::TCP>::Key> _mapping_metis_real;
		std::map<net::NetworkType<net::TCP>::Key, net::NetworkType<net::TCP>::Key> _mapping_real_metis;

		std::mutex _mapLock;

		std::mutex _threadLock;
		std::multimap<uint16_t, std::thread *> threads_;

		std::mutex _deleteSocketsLock;
		std::set<boost::asio::ip::tcp::socket *> _deleteSockets;

		std::vector<net::NetworkType<net::TCP>::Key> _purgeKeys;

		void eraseSocket(net::NetworkType<net::TCP>::Key realKey);
		void workerFunc();

		/**
		 * \brief called after incoming connection. New thread
		 */
		void readFromSocket(boost::asio::ip::tcp::socket * oldSocket);

		void write(const std::vector<uint8_t> & message, boost::asio::ip::tcp::socket * sock);

		void writeImpl(const std::vector<uint8_t> & message, boost::asio::ip::tcp::socket * sock);

		void write();

		void writeHandler(const boost::system::error_code & error, const size_t bytesTransferred);

		void handleAccept(const boost::system::error_code & error, boost::asio::ip::tcp::socket * socket);

		net::NetworkType<net::TCP>::Key real2metis(net::NetworkType<net::TCP>::Key key) {
			auto it = _mapping_real_metis.find(key);
			if (it == _mapping_real_metis.end()) {
				return key;
			}
			return it->second;
		}

		net::NetworkType<net::TCP>::Key metis2real(net::NetworkType<net::TCP>::Key key) {
			auto it = _mapping_metis_real.find(key);
			if (it == _mapping_metis_real.end()) {
				return key;
			}
			return it->second;
		}

		void removeSocket(boost::asio::ip::tcp::socket * sock);

		void purgeSockets();

		TcpWrapper(const TcpWrapper &) = delete;
		TcpWrapper & operator=(const TcpWrapper & rhs) = delete;
	};

} /* namespace tcp */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_TCPWRAPPER_H__ */

/**
 * @}
 */
