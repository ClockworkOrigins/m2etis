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

#include "m2etis/wrapper/udp/UdpWrapper.h"

#include <thread>

#include "m2etis/message/MessageSerialization.h"
#include "m2etis/message/NetworkMessage.h"
#include "m2etis/net/NodeHandle.h"
#include "m2etis/parameters/WrapperParameters.h"
#include "m2etis/util/Logger.h"

namespace m2etis {
namespace wrapper {
namespace udp {

	UdpWrapper::UdpWrapper(const std::string & ownIP, uint16_t listenPort, const std::string & hostIP, uint16_t hostPort) : _initialized(false), _name(ownIP), _hostName(hostIP), _listenPort(listenPort), _hostPort(hostPort), _io_service(), _socket(), _root(), _strand__(_io_service), _outbox(), _work(_io_service), _endpoint(), _remote_endpoint() {
		std::stringstream ss;
		ss << _hostName << ":" << _hostPort;
		_root = net::NetworkType<net::UDP>::Key(ss.str());

		threads_.push_back(new std::thread(std::bind(static_cast<std::size_t(boost::asio::io_service::*)(void)>(&boost::asio::io_service::run), &_strand__.get_io_service())));
		std::thread(std::bind(&UdpWrapper::workerFunc, this)).join();
		_initialized = true;
	}

	UdpWrapper::~UdpWrapper() {
		_initialized = false;
		_io_service.stop();
		for (size_t i = 0; i < threads_.size(); ++i) {
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

	void UdpWrapper::workerFunc() {
		try {
			if (_socket == nullptr) {
				_endpoint = new boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), _listenPort);

				try {
					_socket = new boost::asio::ip::udp::socket(_io_service, *_endpoint);
				} catch(boost::system::system_error & e) {
					M2ETIS_THROW_FAILURE("UdpWrapper - workerFunc", e.what(), e.code().value());
				}
			}
			_remote_endpoint = new boost::asio::ip::udp::endpoint();
			// start receiving again
			boost::system::error_code error;
			_socket->async_receive_from(boost::asio::buffer(recv_buf, parameters::UDPWRAPPER_SIZE), *_remote_endpoint, std::bind(&UdpWrapper::handleReceive, this, std::placeholders::_1, std::placeholders::_2, _remote_endpoint));
		} catch (util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();
		}
	}

	void UdpWrapper::handleReceive(const boost::system::error_code & e, size_t len, boost::asio::ip::udp::endpoint * re) {
		if (e.value() != 0) {
			return;
		}
		std::vector<uint8_t> data(len);
		for (size_t i = 0; i < len; i++) {
			data[i] = recv_buf[i];
		}
		std::string message(data.begin(), data.begin() + std::string::difference_type(len));

		std::stringstream ss;
		ss << re->address().to_string() << ":" << re->port();
		message::Key<message::IPv4KeyProvider> k(ss.str());

		std::thread t(std::bind(&UdpWrapper::handleReceiveMessage, this, _socket, message, re, len));
		workerFunc();
		t.join();
	}

	void UdpWrapper::handleReceiveMessage(boost::asio::ip::udp::socket *, std::string message, boost::asio::ip::udp::endpoint * endpoint, size_t len) {
		std::vector<uint8_t> v(message.begin(), message.begin() + std::string::difference_type(len));
		std::string msgString(v.begin(), v.end());

		try {
			message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg(message::serialization::deserializeNetworkMsg<net::NetworkType<net::UDP>>(msgString));

			msg->sender = net::NetworkType<net::UDP>::Key(endpoint->address().to_string() + ":" + std::to_string(endpoint->port()));

			delete endpoint;

			_callback->deliver(msg);
		} catch (boost::archive::archive_exception & e) {
			std::cout << "Caught exception: " << e.what() << std::endl;
		}
	}

	void UdpWrapper::send(const message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::UDP>>::Ptr_const hint) {
		// TODO: write or delete
		if (!_initialized) {
			M2ETIS_THROW_FAILURE("UdpWrapper - UdpWrapper not initialized", "Call init first!", -1);
		}

		send(msg);
	}

	void UdpWrapper::send(const message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg) {
		if (!_initialized) {
			M2ETIS_THROW_FAILURE("UdpWrapper - UdpWrapper not initialized", "Call init first!", -1);
		}
		try {
			message::Key<message::IPv4KeyProvider> key = msg->receiver;

			std::string msgString = message::serialization::serializeNetworkMsg<message::NetworkMessage<net::NetworkType<net::UDP>>>(msg);
			std::vector<uint8_t> v(msgString.begin(), msgString.end());

			write(v, msg->receiver);
		} catch(util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();
		}
	}

	void UdpWrapper::write(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key) {
		if (_initialized) {
			_strand__.post(std::bind(&UdpWrapper::writeImpl, this, message, key));
		}
	}

	void UdpWrapper::writeImpl(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key) {
		if (_initialized) {
			_outbox.push_back(std::make_pair(message, key));
			if (_outbox.size() > 1) {
				// outstanding async_write
				return;
			}
			this->write();
		}
	}

	void UdpWrapper::write() {
		while (!_outbox.empty() && _initialized) {
			msgPair & message = _outbox[0];
			boost::asio::ip::udp::resolver resolver(_io_service);
			boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), message.second.ipStr(), message.second.portStr());
			boost::asio::ip::udp::endpoint endpoint(*resolver.resolve(query));
			boost::system::error_code err;

			_socket->send_to(boost::asio::buffer(message.first), endpoint);
			_outbox.pop_front();
		}
	}

	void UdpWrapper::registerMessageType(const message::MessageType, const bool) const {
		if (!_initialized) {
			M2ETIS_THROW_FAILURE("UdpWrapper - UdpWrapper not initialized", "Call init first!", -1);
		}
	}

} /* namespace udp */
} /* namespace wrapper */
} /* namespace m2etis */
