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

// refactor!!! Why is boost asio here.
#include "boost/asio.hpp"

#include "m2etis/net/NetworkFactory.h"

#ifdef WITH_SIM
	#include "m2etis/sim/OmNetMediator.h"
#endif

#ifdef WITH_CHIMERA
	#include "m2etis/wrapper/chimera/ChimeraWrapper.h"
#endif

#include "m2etis/wrapper/tcp/TcpWrapper.h"
#include "m2etis/wrapper/udp/UdpWrapper.h"
#include "m2etis/wrapper/clocktcp/clockTcpWrapper.h"

namespace m2etis {
namespace net {

	NetworkFactory::NetworkFactory(const std::string & listenIP, const unsigned short listenPort, const std::string & connectIP, const unsigned short connectPort, pubsub::PubSubSystemEnvironment * pssi) : _listenIP(listenIP), _listenPort(listenPort), _connectIP(connectIP), _connectPort(connectPort), _pssi(pssi),
#ifdef WITH_SIM
		simcontroller_(new NetworkController<NetworkType<OMNET> >(new sim::OmNetMediator(listenIP, listenPort, connectIP, connectPort), pssi)),
#endif /* WITH_SIM */
#ifdef WITH_CHIMERA
		chimeracontroller_(new NetworkController<NetworkType<CHIMERA> >(new wrapper::chimera::ChimeraWrapper(), pssi)), // TODO: (Daniel) change constructor to the one the others are using (after getting it compiling again)!!!
#endif /* WITH_CHIMERA */
		tcpcontroller_(nullptr),
		udpcontroller_(nullptr),
		clocktcpcontroller_(nullptr) {
	}

	NetworkFactory::~NetworkFactory() {
		delete tcpcontroller_;
		delete udpcontroller_;
		delete clocktcpcontroller_;
#ifdef WITH_CHIMERA
		delete chimeracontroller_;
#endif
#ifdef WITH_SIM
		delete simcontroller_;
#endif
}

#ifdef WITH_CHIMERA
	NetworkController<NetworkType<CHIMERA> > * NetworkFactory::createNetworkController(NetworkType<CHIMERA> t) {
		return chimeracontroller_;
	}
#endif
	NetworkController<NetworkType<TCP> > * NetworkFactory::createNetworkController(NetworkType<TCP>) {
		if (tcpcontroller_ == nullptr) {
			tcpcontroller_ = new NetworkController<NetworkType<TCP> >(new wrapper::tcp::TcpWrapper(_listenIP, _listenPort, _connectIP, _connectPort), _pssi);
		}
		return tcpcontroller_;
	}

	NetworkController<NetworkType<UDP> > * NetworkFactory::createNetworkController(NetworkType<UDP>) {
		if (udpcontroller_ == nullptr) {
			udpcontroller_ = new NetworkController<NetworkType<UDP> >(new wrapper::udp::UdpWrapper(_listenIP, _listenPort, _connectIP, _connectPort), _pssi);
		}
		return udpcontroller_;
	}

	NetworkController<NetworkType<clockTCP> > * NetworkFactory::createNetworkController(NetworkType<clockTCP>) {
		if (clocktcpcontroller_ == nullptr) {
			clocktcpcontroller_ = new NetworkController<NetworkType<clockTCP> >(new wrapper::clocktcp::clockTcpWrapper(_listenIP, _listenPort, _connectIP, _connectPort), _pssi);
		}
		return clocktcpcontroller_;
	}
#ifdef WITH_SIM
	NetworkController<NetworkType<OMNET> > * NetworkFactory::createNetworkController(NetworkType<OMNET> t) {
		return simcontroller_;
	}
#endif

} /* namespace net */
} /* namespace m2etis */
