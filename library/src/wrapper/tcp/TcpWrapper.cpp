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

#include "m2etis/wrapper/tcp/TcpWrapper.h"

#include "m2etis/util/Logger.h"

#include "m2etis/message/MessageSerialization.h"

#include "boost/array.hpp"
#include "boost/bind.hpp"
#include "boost/thread.hpp"

namespace m2etis {
namespace wrapper {
namespace tcp {

	TcpWrapper::TcpWrapper(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort) : _initialized(true), _local(listenIP + ":" + std::to_string(listenPort)), _rendezvouz(connectIP + ":" + std::to_string(connectPort)), _io_service(), _acceptor(), _sockets(), _strand__(_io_service), _outbox(), _work(new boost::asio::io_service::work(_io_service)), _mapping_metis_real(), _mapping_real_metis(), _mapLock(), _threadLock(), threads_() {
		threads_.insert(std::make_pair(0, new boost::thread(boost::bind(&boost::asio::io_service::run, &_strand__.get_io_service()))));
		boost::thread(boost::bind(&TcpWrapper::workerFunc, this)).join();
	}

	TcpWrapper::~TcpWrapper() {
		_initialized = false;
		_io_service.stop();
		delete _work;
		_acceptor->close();
		lock_.lock();
		for (std::map<message::Key<message::IPv4KeyProvider>, boost::asio::ip::tcp::socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
			if (it->second != nullptr) {
				if (it->second->is_open()) {
					it->second->cancel();
					it->second->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
				}
				it->second->close();
				delete it->second;
			}
		}
		_sockets.clear();
		lock_.unlock();
		_threadLock.lock();
		for (std::pair<uint16_t, boost::thread *> p : threads_) {
			p.second->interrupt();
			if (p.second->joinable()) {
				p.second->join();
			}
			delete p.second;
		}
		_threadLock.unlock();
		delete _acceptor;
	}

	void TcpWrapper::workerFunc() {
		try {
			_acceptor = new boost::asio::ip::tcp::acceptor(_io_service);
			boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), _local.getPort());
			boost::system::error_code error;
			_acceptor->open(endpoint.protocol(), error);

			if (error) {
				M2ETIS_THROW_FAILURE("TcpWrapper - Error opening port acceptor", error.message(), error.value());
			}
			_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), error);

			if (error) {
				M2ETIS_THROW_FAILURE("TcpWrapper - Error setting option", error.message(), error.value());
			}
			_acceptor->bind(endpoint, error);

			if (error) {
				M2ETIS_THROW_FAILURE("TcpWrapper - Error binding to port", error.message(), error.value());
			}
			_acceptor->listen(3, error);
			if (error) {
				M2ETIS_THROW_FAILURE("TcpWrapper - Error listening on port", error.message(), error.value());
			}

			boost::asio::ip::tcp::socket * newSocket = new boost::asio::ip::tcp::socket(_io_service);
			_acceptor->async_accept(*newSocket, boost::bind(&TcpWrapper::handleAccept, this, boost::asio::placeholders::error, newSocket));
			boost::mutex::scoped_lock l(lock_);
			_sockets.insert(std::make_pair(_local, newSocket));
		} catch(util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();
		}
	}

	void TcpWrapper::handleAccept(const boost::system::error_code & error, boost::asio::ip::tcp::socket * socket) {
		if (error) {
			M2ETIS_THROW_FAILURE("TcpWrapper - Error accepting connection", error.message(), error.value());
		}

		message::Key<message::IPv4KeyProvider> k(socket->remote_endpoint().address().to_string() + ":" + std::to_string(socket->remote_endpoint().port()));

		{
			boost::mutex::scoped_lock l(lock_);
			_sockets[k] = socket;
			std::lock_guard<std::mutex> lg(_threadLock);
			threads_.insert(std::make_pair(0, new boost::thread(boost::bind(&TcpWrapper::readFromSocket, this, socket))));
		}

		// Listen for new connection
		boost::asio::ip::tcp::socket * newSocket = new boost::asio::ip::tcp::socket(_io_service);
		_sockets[_local] = newSocket;
		_acceptor->async_accept(*newSocket, boost::bind(&TcpWrapper::handleAccept, this, boost::asio::placeholders::error, newSocket));
	}

	void TcpWrapper::send(const message::NetworkMessage<net::NetworkType<net::TCP>>::Ptr msg, net::NodeHandle<net::NetworkType<net::TCP>>::Ptr_const hint) {
		// TODO: (???) write or delete
		if (!_initialized) {
			// M2ETIS_THROW_FAILURE("TcpWrapper - TcpWrapper not initialized", "Call init first!", -1);
		}

		send(msg);
	}

	void TcpWrapper::send(const message::NetworkMessage<net::NetworkType<net::TCP>>::Ptr msg) {
		if (!_initialized) {
			// M2ETIS_THROW_FAILURE("TcpWrapper - TcpWrapper not initialized", "Call init first!", -1);
		}

		try {
			std::string ser = message::serialization::serializeNetworkMsg<message::NetworkMessage<net::NetworkType<net::TCP>>>(msg);
			std::vector<uint8_t> v(ser.begin(), ser.end());

			std::vector<uint8_t> delim = {0x01, 0x02, 0x03, 0x04, 0x05};
			for (size_t i = 0; i < delim.size(); ++i) {
				v.push_back(delim[i]);
			}

			for (size_t i = 0; i < v.size() - delim.size(); ++i) {
				size_t j;
				for (j = 0; j < delim.size(); ++j) {
					if (v[i + j] != delim[j]) {
						break;
					}
				}
				assert(j < delim.size());
			}

			boost::system::error_code err;
			
			// Check if connection already open
			net::NetworkType<net::TCP>::Key metisKey = msg->receiver;
			_mapLock.lock();
			net::NetworkType<net::TCP>::Key realKey = metis2real(metisKey);
			_mapLock.unlock();
			boost::mutex::scoped_lock l(lock_);
			if (_sockets.find(realKey) == _sockets.end()) {
				M2ETIS_LOG_DEBUG("TcpWrapper", "creating new socket: " << realKey.ipStr() << ":" << realKey.portStr());
				boost::asio::ip::tcp::resolver resolver(_io_service);
				boost::asio::ip::tcp::socket * socket = new boost::asio::ip::tcp::socket(_io_service);
				boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), realKey.ipStr(), realKey.portStr());
				boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
				boost::asio::ip::tcp::resolver::iterator end;
				boost::system::error_code error = boost::asio::error::host_not_found;

				socket->connect(*it++, error);

				if (error) {
					socket->close();
					delete socket;
					M2ETIS_THROW_FAILURE("TcpWrapper - Couldn't connect", metisKey.toStr(), error.value());
				}

				_sockets[realKey] = socket;
				std::lock_guard<std::mutex> lg(_threadLock);
				threads_.insert(std::make_pair(0, new boost::thread(boost::bind(&TcpWrapper::readFromSocket, this, socket))));
			}
			if (_sockets.find(realKey)->second == nullptr) {
				return;
			}
			write(v, _sockets.find(realKey)->second);
		} catch (util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();

			// remove socket from map
			net::NetworkType<net::TCP>::Key metisKey = msg->receiver;
			boost::mutex::scoped_lock sl(_mapLock);
			net::NetworkType<net::TCP>::Key realKey = metis2real(metisKey);

			boost::mutex::scoped_lock l(lock_);
			if (_sockets.find(realKey) != _sockets.end()) {
				_sockets[realKey]->close();
				delete _sockets[realKey];
				_sockets[realKey] = nullptr;
				std::lock_guard<std::mutex> lg(_threadLock);
				threads_.insert(std::make_pair(1, new boost::thread(boost::bind(&TcpWrapper::eraseSocket, this, realKey))));
			}
		} catch (boost::archive::archive_exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	void TcpWrapper::registerMessageType(const message::MessageType, const bool) const {
		if (!_initialized) {
			M2ETIS_THROW_FAILURE("TcpWrapper - TcpWrapper not initialized", "Call init first!", -1);
		}
	}

	void TcpWrapper::readFromSocket(boost::asio::ip::tcp::socket * socket) {
		net::NetworkType<net::TCP>::Key realKey(socket->remote_endpoint().address().to_string() + ":" + std::to_string(socket->remote_endpoint().port()));
		net::NetworkType<net::TCP>::Key metisKey = real2metis(realKey);
		try {
			boost::asio::streambuf buf;
			boost::system::error_code error;

			while (_initialized) {
				boost::this_thread::interruption_point();
				// read until message delemiter appears
				char delim[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x00 };
				size_t len = boost::asio::read_until(*socket, buf, delim, error);
				boost::this_thread::interruption_point();
				if (error) {
					M2ETIS_THROW_FAILURE("TcpWrapper - An error receiving a message occured", metisKey.toStr(), error.value());
				} else {
					const char * m = boost::asio::buffer_cast<const char *>(buf.data());
					std::string message(m, m + len);
					buf.consume(len);

					message::NetworkMessage<net::NetworkType<net::TCP>>::Ptr msg = message::NetworkMessage<net::NetworkType<net::TCP>>::Ptr(message::serialization::deserializeNetworkMsg<net::NetworkType<net::TCP>>(message));
					if (msg->sender == net::NetworkType<net::TCP>::Key()) {
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
			}
		} catch (util::SystemFailureException & e) {
			e.writeLog();
			e.PassToMain();

			boost::mutex::scoped_lock l(lock_);
			// TODO simplify
			for (std::map<net::NetworkType<net::TCP>::Key, boost::asio::ip::tcp::socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
				if (it->second == socket) {
					socket->close();
					delete socket;
					it->second = nullptr;
					std::lock_guard<std::mutex> lg(_threadLock);
					threads_.insert(std::make_pair(1, new boost::thread(boost::bind(&TcpWrapper::eraseSocket, this, it->first))));
					break;
				}
			}
		} catch (boost::archive::archive_exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	void TcpWrapper::write(const std::vector<uint8_t> & message, boost::asio::ip::tcp::socket * sock) {
		if (_initialized) {
			_strand__.post(boost::bind(&TcpWrapper::writeImpl, this, message, sock));
		}
	}

	void TcpWrapper::writeImpl(const std::vector<uint8_t> & message, boost::asio::ip::tcp::socket * sock) {
		if (_initialized) {
			bool goOn = false;
			{
				boost::mutex::scoped_lock l(lock_);
				for(std::map<net::NetworkType<net::TCP>::Key, boost::asio::ip::tcp::socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
					if (it->second == sock) {
						goOn = true;
						break;
					}
				}
			}
			if (goOn) {
				_outbox.push_back(make_pair(message, sock));
				if (_outbox.size() > 1) {
					// outstanding async_write
					return;
				}
				this->write();
			}
		}
	}

	void TcpWrapper::write() {
		if (_initialized) {
			assert(!_outbox.empty());
			const msgPair & message = _outbox[0];
			bool goOn = false;
			{
				boost::mutex::scoped_lock l(lock_);
				for(std::map<net::NetworkType<net::TCP>::Key, boost::asio::ip::tcp::socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
					if (it->second == message.second) {
						goOn = true;
						break;
					}
				}
			}
			if (goOn) {
				boost::asio::async_write(*(message.second), boost::asio::buffer(&(message.first[0]), message.first.size()), _strand__.wrap(boost::bind(&TcpWrapper::writeHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
			}
		}
	}

	void TcpWrapper::writeHandler(const boost::system::error_code & error, const size_t) {
		if (_initialized) {
			boost::asio::ip::tcp::socket * sock = _outbox.front().second;
			_outbox.pop_front();

			if (error) {
				boost::mutex::scoped_lock l(lock_);
				for(std::map<net::NetworkType<net::TCP>::Key, boost::asio::ip::tcp::socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
					if (it->second == sock) {
						sock->close();
						delete sock;
						it->second = nullptr;
						break;
					}
				}
			}

			if (!_outbox.empty()) {
				// more messages to send
				this->write();
			}
		}
	}

	void TcpWrapper::eraseSocket(net::NetworkType<net::TCP>::Key realKey) {
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		boost::this_thread::interruption_point();
		boost::mutex::scoped_lock sl(_mapLock);
		boost::this_thread::interruption_point();
		_mapping_metis_real.erase(real2metis(realKey));
		_mapping_real_metis.erase(realKey);
		boost::mutex::scoped_lock l(lock_);
		_sockets.erase(realKey);
	}

} /* namespace tcp */
} /* namespace wrapper */
} /* namespace m2etis */
