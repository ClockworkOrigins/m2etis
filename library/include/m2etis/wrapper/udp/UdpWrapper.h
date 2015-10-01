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

    class UdpWrapper : public net::NetworkInterface<net::NetworkType<net::UDP> > {
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

		void workerFunc();

		void handleReceive(boost::asio::ip::udp::socket * socket, std::string message, boost::asio::ip::udp::endpoint * endpoint, size_t len);
		void handleReceive(const boost::system::error_code & error, size_t len, boost::asio::ip::udp::endpoint * re);

		UdpWrapper(const UdpWrapper &) = delete;
		UdpWrapper & operator=(const UdpWrapper & rhs) = delete;

		boost::asio::io_service::strand _strand__;
		// buffers messages till they got sent
		std::deque<msgPair> _outbox;
		boost::asio::io_service::work _work; // keeps the io_service running

		boost::asio::ip::udp::endpoint * _endpoint;
		boost::asio::ip::udp::endpoint * _remote_endpoint;

	public:
		UdpWrapper(const std::string & ownIP, uint16_t listenPort, const std::string & hostIP, uint16_t hostPort) :
			_initialized(false),
			_name("127.0.0.1"),
			_hostName("127.0.0.1"),
			_listenPort(12345),
			_hostPort(12345),
			_io_service(),
			_socket(),
			_root(),
			_strand__(_io_service),
			_outbox(),
			_work(_io_service),
			_endpoint(),
			_remote_endpoint() {
				_hostName = hostIP;
				_hostPort = hostPort;
				_name = ownIP;
				_listenPort = listenPort;

				std::stringstream ss;
				ss << _hostName << ":" << _hostPort;
				_root = net::NetworkType<net::UDP>::Key(ss.str());

				threads_.push_back(new boost::thread(boost::bind(&boost::asio::io_service::run, &_strand__.get_io_service())));
				boost::this_thread::sleep(boost::posix_time::milliseconds(100));
				threads_.push_back(new boost::thread(boost::bind(&UdpWrapper::workerFunc, this)));
				boost::this_thread::sleep(boost::posix_time::milliseconds(100));
				_initialized = true;
		}

		~UdpWrapper() {
			_initialized = false;
			_io_service.stop();
			for (size_t i = 0; i < threads_.size(); ++i) {
				threads_[i]->interrupt();
				threads_[i]->join();
				delete threads_[i];
			}
			if (_socket != nullptr) {
				_socket->close();
				delete _socket;
			}
			delete _endpoint;
			delete _remote_endpoint;
		}

		/**
		 * \brief ruft die andere send methode auf
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::UDP>>::Ptr_const hint);

		/**
		 * \brief Schickt type(Dann Trennung durch "/") und payload(als char*) in einem stream buff an to (ip:port )
		 */
		void send(const message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg);

		/**
		 * \brief Gibt Pointer mit eigenen Daten zurück
		 * \return Im Pointer steht der _name, _port und key aus (_name:_port)
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
		 * \brief Momentan geschieht hier nichts. Es werden alle MsgTypes delivered. Untescheidung muss eine Schicht drüber gemacht werden.
		 */
		void registerMessageType(const message::MessageType type, const bool ack) const;

		net::NetworkType<net::UDP>::Key getRoot() const {
			return _root;
		}

		void write(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key);

		void writeImpl(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key);

		void write() {
			while(!_outbox.empty() && _initialized) {
				msgPair & message = _outbox[0];
				boost::asio::ip::udp::resolver resolver(_io_service);
				boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), message.second.ipStr(), message.second.portStr());
				boost::asio::ip::udp::endpoint * endpoint = new boost::asio::ip::udp::endpoint(*resolver.resolve(query));
				boost::system::error_code err;

				_socket->send_to(boost::asio::buffer(message.first), *endpoint);
				_outbox.pop_front();
			}
		}
	};

} /* namespace udp */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_UDP_UDPWRAPPER_H__ */

