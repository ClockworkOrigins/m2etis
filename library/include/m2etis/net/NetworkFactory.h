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
 * \addtogroup net
 * @{
 */

#ifndef __M2ETIS_NET_NETWORKFACTORY_H__
#define __M2ETIS_NET_NETWORKFACTORY_H__

#include "m2etis/util/Exceptions.h"

#include "m2etis/message/key/providers/IPv4KeyProvider.h"
#include "m2etis/net/NetworkController.h"
#include "m2etis/net/NetworkName.h"
#include "m2etis/net/NetworkType.h"

namespace m2etis {
namespace pubsub {
	class PubSubSystemEnvironment;
} /* namespace pubsub */
namespace net {

	/**
	 * \class NetworkFactory
	 * \brief NetworkFactory
	 *
	 * Long Desc
	 */
	class NetworkFactory {
	public:
		NetworkFactory(const std::string & listenIP, const unsigned short listenPort, const std::string & connectIP, const unsigned short connectPort, pubsub::PubSubSystemEnvironment * pssi);

		~NetworkFactory();

		/**
		 * \brief creates NetworkController
		 * \param[in] NetworkType NetworkType
		 * \return Returns a NetworkController object
		 */
		template<class NetworkType>
		NetworkController<NetworkType> * createNetworkController(NetworkType) {
			M2ETIS_THROW_FAILURE("NetworkFactory", "Wrong Usage of Network Factory! Call with existing NetworkType!", 1);
		}

#ifdef WITH_CHIMERA
		NetworkController<NetworkType<CHIMERA> > * createNetworkController(NetworkType<CHIMERA>);
#endif

		NetworkController<NetworkType<TCP>> * createNetworkController(NetworkType<TCP>);
		NetworkController<NetworkType<UDP>> * createNetworkController(NetworkType<UDP>);
		NetworkController<NetworkType<clockTCP>> * createNetworkController(NetworkType<clockTCP>);

#ifdef WITH_SIM
		NetworkController<NetworkType<OMNET>> * createNetworkController(NetworkType<OMNET>);
#endif

	private:
		std::string _listenIP;
		unsigned short _listenPort;
		std::string _connectIP;
		unsigned short _connectPort;
		pubsub::PubSubSystemEnvironment * _pssi;
#ifdef WITH_CHIMERA
		NetworkController<NetworkType<CHIMERA>> * chimeracontroller_;
#endif /* WITH_CHIMERA */

#ifdef WITH_SIM
		NetworkController<NetworkType<OMNET>> * simcontroller_;
#endif

		NetworkController<NetworkType<TCP>> * tcpcontroller_;
		NetworkController<NetworkType<UDP>> * udpcontroller_;
		NetworkController<NetworkType<clockTCP>> * clocktcpcontroller_;
	};

} /* namespace net */
} /* namespace m2etis */

#endif /* __M2ETIS_NET_NETWORKFACTORY_H__ */

/** @}
 */
