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

#include "m2etis/wrapper/clocktcp/clockTcpWrapper.h"

#include <thread>

#include "m2etis/util/Logger.h"

#include "m2etis/message/MessageSerialization.h"

#include "clockUtils/sockets/TcpSocket.h"

#include "boost/bind.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace wrapper {
namespace clocktcp {

	clockTcpWrapper::clockTcpWrapper(const std::string & listenIP, const uint16_t listenPort, const std::string &, const uint16_t) : _initialized(true), _local(listenIP + ":" + std::to_string(listenPort)), _lock(), _sockets(), _mapping_metis_real(), _mapping_real_metis(), _mapLock(), _threads() {
		clockUtils::sockets::TcpSocket * newSocket = new clockUtils::sockets::TcpSocket();
		newSocket->listen(_local.getPort(), 100, true, [this](clockUtils::sockets::TcpSocket * socket, clockUtils::ClockError err) {
			if (err == clockUtils::ClockError::SUCCESS) {
				message::Key<message::IPv4KeyProvider> k(socket->getRemoteIP() + ":" + std::to_string(socket->getRemotePort()));

				{
					boost::mutex::scoped_lock l(_lock);
					_sockets[k] = socket;
				}
				_threads.insert(std::make_pair(1, new std::thread(std::bind(&clockTcpWrapper::readFromSocket, this, socket))));
			}
		});
		boost::mutex::scoped_lock l(_lock);
		_sockets[_local] = newSocket;
	}

	clockTcpWrapper::~clockTcpWrapper() {
		_initialized = false;
		_lock.lock();
		for (std::map<message::Key<message::IPv4KeyProvider>, clockUtils::sockets::TcpSocket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
			if (it->second != nullptr) {
				clockUtils::sockets::TcpSocket * tmp = it->second;
				tmp->close();
				delete tmp;
			}
		}
		_sockets.clear();
		_lock.unlock();
		for (std::pair<uint16_t, std::thread *> p : _threads) {
			p.second->join();
			delete p.second;
		}
	}

	void clockTcpWrapper::send(const message::NetworkMessage<net::NetworkType<net::clockTCP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::clockTCP>>::Ptr_const /* hint */) {
		// TODO: (???) write or delete
		if (!_initialized) {
			// M2ETIS_THROW_FAILURE("clockTcpWrapper - clockTcpWrapper not initialized", "Call init first!", -1);
		}

		try {
			std::string ser = message::serialization::serializeNetworkMsg<message::NetworkMessage<net::NetworkType<net::clockTCP>>>(msg);
			// Check if connection already open
			net::NetworkType<net::clockTCP>::Key metisKey = msg->receiver;

			_mapLock.lock();
			net::NetworkType<net::clockTCP>::Key realKey = metis2real(metisKey);
			_mapLock.unlock();

			boost::mutex::scoped_lock l(_lock);
			auto it = _sockets.find(realKey);
			if (it == _sockets.end()) {
				M2ETIS_LOG_DEBUG("clockTcpWrapper", "creating new socket: " << realKey.ipStr() << ":" << realKey.portStr());
				clockUtils::sockets::TcpSocket * socket = new clockUtils::sockets::TcpSocket();
				clockUtils::ClockError error = socket->connectToIP(realKey.ipStr(), realKey.getPort(), 2000);

				if (error != clockUtils::ClockError::SUCCESS) {
					socket->close();
					delete socket;
					M2ETIS_THROW_FAILURE("clockTcpWrapper - Couldn't connect", metisKey.toStr(), int(error));
				}

				it = _sockets.insert(std::make_pair(realKey, socket)).first;
				_threads.insert(std::make_pair(1, new std::thread(std::bind(&clockTcpWrapper::readFromSocket, this, socket))));
			}
			if (it->second == nullptr) {
				return;
			}
			it->second->writePacketAsync(std::vector<uint8_t>(ser.begin(), ser.end()));
		} catch (util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();

			// remove socket from map
			net::NetworkType<net::clockTCP>::Key metisKey = msg->receiver;
			boost::mutex::scoped_lock sl(_mapLock);
			net::NetworkType<net::clockTCP>::Key realKey = metis2real(metisKey);

			if (_sockets.find(realKey) != _sockets.end()) {
				_sockets[realKey]->close();
				delete _sockets[realKey];
				_sockets[realKey] = nullptr;
				_threads.insert(std::make_pair(1, new std::thread(std::bind(&clockTcpWrapper::eraseSocket, this, realKey))));
			}
		} catch (boost::archive::archive_exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	void clockTcpWrapper::registerMessageType(const message::MessageType, const bool) const {
		if (!_initialized) {
			M2ETIS_THROW_FAILURE("clockTcpWrapper - clockTcpWrapper not initialized", "Call init first!", -1);
		}
	}

	void clockTcpWrapper::readFromSocket(clockUtils::sockets::TcpSocket * socket) {
		net::NetworkType<net::clockTCP>::Key realKey(socket->getRemoteIP() + ":" + std::to_string(socket->getRemotePort()));
		net::NetworkType<net::clockTCP>::Key metisKey = real2metis(realKey);
		try {
			while (_initialized) {
				boost::this_thread::interruption_point();

				std::string message;
				clockUtils::ClockError error = socket->receivePacket(message);

				if (error != clockUtils::ClockError::SUCCESS) {
					M2ETIS_THROW_FAILURE("clockTcpWrapper - readFromSocket", "error receiving a message", int(error));
				}
				message::NetworkMessage<net::NetworkType<net::clockTCP>>::Ptr msg = message::NetworkMessage<net::NetworkType<net::clockTCP>>::Ptr(message::serialization::deserializeNetworkMsg<net::NetworkType<net::clockTCP>>(message));
				if (msg->sender == net::NetworkType<net::clockTCP>::Key()) {
					// no sender set, use own mapping
					msg->sender = real2metis(realKey);
				} else {
					// sender set, create mapping
					metisKey = msg->sender;
					boost::mutex::scoped_lock sl(_mapLock);
					_mapping_metis_real[metisKey] = realKey;
					_mapping_real_metis[realKey] = metisKey;
				}
				_callback->deliver(msg);
			}
		} catch (util::SystemFailureException & e) {
			if (_initialized) {
				e.writeLog();
				e.PassToMain();

				boost::mutex::scoped_lock l(_lock);
				// TODO simplify
				for (std::map<net::NetworkType<net::clockTCP>::Key, clockUtils::sockets::TcpSocket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
					if (it->second == socket) {
						socket->close();
						delete socket;
						it->second = nullptr;
						_threads.insert(std::make_pair(1, new std::thread(std::bind(&clockTcpWrapper::eraseSocket, this, realKey))));
						break;
					}
				}
			}
		} catch (boost::archive::archive_exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	net::NetworkType<net::clockTCP>::Key clockTcpWrapper::real2metis(net::NetworkType<net::clockTCP>::Key key) {
		auto it = _mapping_real_metis.find(key);
		if (it == _mapping_real_metis.end()) {
			return key;
		}
		return it->second;
	}

	net::NetworkType<net::clockTCP>::Key clockTcpWrapper::metis2real(net::NetworkType<net::clockTCP>::Key key) {
		auto it = _mapping_metis_real.find(key);
		if (it == _mapping_metis_real.end()) {
			return key;
		}
		return it->second;
	}

	void clockTcpWrapper::eraseSocket(net::NetworkType<net::clockTCP>::Key realKey) {
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		boost::this_thread::interruption_point();
		boost::mutex::scoped_lock sl(_mapLock);
		_mapping_metis_real.erase(real2metis(realKey));
		_mapping_real_metis.erase(realKey);
		boost::mutex::scoped_lock l(_lock);
		_sockets.erase(realKey);
	}

} /* namespace clocktcp */
} /* namespace wrapper */
} /* namespace m2etis */
