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

#ifndef __M2ETIS_WRAPPER_CHIMERA_CHIMERAWRAPPER_H__
#define __M2ETIS_WRAPPER_CHIMERA_CHIMERAWRAPPER_H__

#include <string>

#include "m2etis/message/key/providers/SHA1KeyProvider.h"
#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"
#include "m2etis/net/NodeHandle.h"

namespace m2etis {
namespace wrapper {
namespace chimera {

    class ChimeraWrapperImpl;

	/**
	 * @brief The C++-wrapper around chimera
	 */
	class ChimeraWrapper : public net::NetworkInterface<net::NetworkType<net::CHIMERA>> {
	private:
		ChimeraWrapperImpl * _impl;
		ChimeraWrapper(const ChimeraWrapper & rhs);

		ChimeraWrapper & operator=(const ChimeraWrapper & rhs);

	public:
		enum { keysize = 40 };
		ChimeraWrapper();
		~ChimeraWrapper();

		void init(const int port);

		void init(const int port, const std::string & known_hostname, const int know_hostport);

		net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr getSelfNodeHandle() const;

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> local_lookup(const net::NetworkType<net::CHIMERA>::Key & key, const int num, const bool safe) const;

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> neighborSet(const int num) const;

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> replicaSet(const net::NetworkType<net::CHIMERA>::Key & key, const int maxRank) const;

		bool isResponsibleFor(const net::NetworkType<net::CHIMERA>::Key & key, const net::NetworkType<net::CHIMERA>::Key & node) const;

		bool range(const net::NodeHandle<net::NetworkType<net::CHIMERA>>::Ptr node, int rank, const net::NetworkType<net::CHIMERA>::Key & lkey, const  net::NetworkType<net::CHIMERA>::Key & rkey) const;

		void registerMessageType(const message::MessageType type, const bool ack) const;

		void send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg, typename net::NodeHandle<net::NetworkType<net::CHIMERA>>::Ptr_const hint);

		void send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg);
	};

} /* namespace chimera */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_CHIMERA_CHIMERAWRAPPER_H__ */
