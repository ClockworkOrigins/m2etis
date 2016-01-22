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

#include "m2etis/wrapper/chimera/ChimeraWrapper.h"
#include "m2etis/wrapper/chimera/ChimeraWrapperImpl.h"

namespace m2etis {
namespace wrapper {
namespace chimera {

	ChimeraWrapper::ChimeraWrapper() : _impl(new ChimeraWrapperImpl()) {
	}

	ChimeraWrapper::~ChimeraWrapper()  {
		delete _impl;
	}

	void ChimeraWrapper::init(const int port) {
		_impl->init(port);
	}

	void ChimeraWrapper::init(const int port, const std::string & known_hostname, const int know_hostport) {
		_impl->init(port, known_hostname, know_hostport);
	}

	void ChimeraWrapper::send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg, typename net::NodeHandle<net::NetworkType<net::CHIMERA>>::Ptr_const hint) {
		_impl->send(msg, hint);
	}

	void ChimeraWrapper::send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg) {
		_impl->send(msg);
	}

	net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr ChimeraWrapper::getSelfNodeHandle() const {
		return _impl->getSelfNodeHandle();
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapper::local_lookup(const net::NetworkType<net::CHIMERA>::Key & key, const int num, const bool safe) const {
		return _impl->local_lookup(key, num, safe);
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapper::neighborSet(const int num) const {
		return _impl->neighborSet(num);
	}

	bool ChimeraWrapper::isResponsibleFor(const net::NetworkType<net::CHIMERA>::Key & key, const net::NetworkType<net::CHIMERA>::Key & node) const {
		return _impl->isResponsibleFor(key, node);
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapper::replicaSet(const net::NetworkType<net::CHIMERA>::Key & key, int maxRank) const {
		return _impl->replicaSet(key, maxRank);
	}

	bool ChimeraWrapper::range(const net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node, const int rank, const net::NetworkType<net::CHIMERA>::Key & lkey, const net::NetworkType<net::CHIMERA>::Key & rkey) const {
		return _impl->range(node, rank, lkey, rkey);
	}

	void ChimeraWrapper::registerMessageType(const message::MessageType type, const bool ack) const {
		_impl->registerMessageType(type, ack);
	}

} /* namespace chimera */
} /* namespace wrapper */
} /* namespace m2etis */
