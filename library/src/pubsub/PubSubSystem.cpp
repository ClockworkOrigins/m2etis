/*
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

#include "m2etis/pubsub/PubSubSystem.h"

#include "m2etis/pubsub/PubSubSystemEnvironment.h"

/*
* This file generates the explicit type instantiations for the derived message types. These are used by boost serialization to work on the messages polymorphic.
* It uses the channel configuration in order to derive the types for messages.
*/

#include "m2etis/message/MessageSerialization.h"
#include "m2etis/pubsub/config/Strategies.h"

#include "m2etis/config/GeneratedMessageConfiguration.h"

#include "boost/asio/error.hpp"

namespace m2etis {
namespace pubsub {

	PubSubSystem::PubSubSystem(const std::string & listenIP, const uint16_t listenPort, const std::string & connectIP, const uint16_t connectPort, const std::vector<std::string> & rootList) : _pssi(new PubSubSystemEnvironment(listenIP, listenPort, connectIP, connectPort)), channels_(new ChannelConfiguration(listenIP, listenPort, connectIP, connectPort, _pssi, rootList)), initialized(true), _exceptionCallbacks(5, std::vector<boost::function<void(const std::string &)>>()), _running(true) {
		exceptionID_ = _pssi->scheduler_.runRepeated(500000, boost::bind(&PubSubSystem::exceptionLoop, this), 4);
#ifdef WITH_LOGGING
		util::log::initializeLogging();
#endif
	}

	PubSubSystem::~PubSubSystem() {
		_running = false;
		_pssi->scheduler_.stop(exceptionID_);
		_pssi->scheduler_.Stop();
		delete channels_;
		delete _pssi;
		_exceptionCallbacks.clear();
#ifdef WITH_LOGGING
		util::log::shutdownLogging();
#endif
	}

	void PubSubSystem::registerExceptionCallback(exceptionEvents e, boost::function<void(const std::string &)> _ptr) {
		_exceptionCallbacks[e].push_back(_ptr);
	}

	bool PubSubSystem::exceptionLoop() {
		while (!util::ExceptionQueue::isEmpty() && _running) {
			util::loginfo info = util::ExceptionQueue::dequeue();
			util::SystemFailureException e(info.module, info.message, info.file, info.line, info.errorcode);
			e.writeLog();

			exceptionEvents ev = UNDEFINED;

			if (info.errorcode == boost::asio::error::connection_refused) {
				 ev = CONNECTION_FAILED;
			} else if (info.errorcode == boost::asio::error::eof) {
				ev = CONNECTION_CLOSED;
			} else if (info.errorcode == boost::asio::error::address_in_use) {
				ev = BINDFAILURE;
			} else {
				M2ETIS_LOG_DEBUG("PSS", info.message);
			}

			if (_running && !_exceptionCallbacks[ev].empty()) {
				for (boost::function<void(const std::string &)> f : _exceptionCallbacks[ev]) {
					boost::thread(boost::bind(f, info.message));
				}
			}
		}

		return _running;
	}

} /* namespace pubsub */
} /* namespace m2etis */
