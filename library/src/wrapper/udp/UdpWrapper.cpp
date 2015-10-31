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
 * look header for docu
 */

#include "m2etis/wrapper/udp/UdpWrapper.h"

#include "m2etis/message/MessageSerialization.h"
#include "m2etis/message/NetworkMessage.h"
#include "m2etis/net/NodeHandle.h"
#include "m2etis/parameters/WrapperParameters.h"
#include "m2etis/util/Logger.h"

#include "boost/array.hpp"      // nur fuer array buff anstatt streambuf
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

namespace m2etis {
namespace wrapper {
namespace udp {

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
			_socket->async_receive_from(boost::asio::buffer(recv_buf, parameters::UDPWRAPPER_SIZE), *_remote_endpoint, boost::bind(&UdpWrapper::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, _remote_endpoint));
		} catch (util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();
		}
	}

	void UdpWrapper::handleReceive(const boost::system::error_code &, size_t len, boost::asio::ip::udp::endpoint * re) {
		std::vector<uint8_t> data(len);
		for (size_t i = 0; i < len; i++) {
			data[i] = recv_buf[i];
		}
		std::string message(data.begin(), data.begin() + std::string::difference_type(len));

		std::stringstream ss;
		ss << re->address().to_string() << ":" << re->port();
		message::Key<message::IPv4KeyProvider> k(ss.str());

		// TODO: (Daniel) is this really necessary? shouldn't workerFunc line 53 loop internally? check with integration test ore bigger channel test (more clients subscribing)
		boost::thread t(boost::bind(&UdpWrapper::handleReceive, this, _socket, message, re, len));
		workerFunc();
	}

	void UdpWrapper::handleReceive(boost::asio::ip::udp::socket *, std::string message, boost::asio::ip::udp::endpoint * endpoint, size_t len) {
		std::vector<uint8_t> v(message.begin(), message.begin() + std::string::difference_type(len));
		std::string msgString(v.begin(), v.end());

		message::NetworkMessage<net::NetworkType<net::UDP>>::Ptr msg(message::serialization::deserializeNetworkMsg<net::NetworkType<net::UDP>>(msgString));

		msg->sender = net::NetworkType<net::UDP>::Key(endpoint->address().to_string() + ":" + std::to_string(endpoint->port()));

		_callback->deliver(msg);
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
		_strand__.post(boost::bind(&UdpWrapper::writeImpl, this, message, key));
	}

	void UdpWrapper::writeImpl(const std::vector<uint8_t> & message, message::Key<message::IPv4KeyProvider> key) {
		_outbox.push_back(std::make_pair(message, key));
		if (_outbox.size() > 1) {
			// outstanding async_write
			return;
		}
		this->write();
	}

	void UdpWrapper::registerMessageType(const message::MessageType, const bool) const {
		if (!_initialized) {
			M2ETIS_THROW_FAILURE("UdpWrapper - UdpWrapper not initialized", "Call init first!", -1);
		}
	}

} /* namespace udp */
} /* namespace wrapper */
} /* namespace m2etis */
