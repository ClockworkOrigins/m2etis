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

#ifndef __M2ETIS_WRAPPER_CHIMERA_CHIMERAWRAPPERIMPL_H__
#define __M2ETIS_WRAPPER_CHIMERA_CHIMERAWRAPPERIMPL_H__

#include <stdexcept>
#include <memory>

#include "m2etis/util/SystemParameters.h"

#include "m2etis/net/NetworkCallbackInterface.h"
#include "m2etis/net/NetworkInterface.h"
#include "m2etis/net/NodeHandle.h"

#include "m2etis/wrapper/chimera/ChimeraWrapper.h"

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/thread.hpp"

extern "C" {
    #include "chimera/chimera.h"
}

namespace m2etis {
namespace wrapper {
namespace chimera {

	class ChimeraWrapper;

	/**
	 * @brief The C++-wrapper around chimera
	 * @details This class will have a global instance due to chimeras need for functionpointers for upcalling
	 * into the application. Using static methods for that …
	 */
	class ChimeraWrapperImpl {
		friend class ChimeraWrapper;

	private:
		const static int CHIMERA_MESSAGE_TYPE_BASE = 15;

		ChimeraWrapperImpl() : _cb(nullptr), _initialized(false), _me(new ChimeraHost), _state(new ChimeraState), periodic_(boost::posix_time::milliseconds(3000)), purge_distance_(boost::posix_time::milliseconds(6000)), neighbors_(), neighbors_mutex_(), purging_(true), purger_(boost::bind(&ChimeraWrapperImpl::manageHeartbeats, this)), map_mutex_(), map_() {
			_impl = this;
		}

		ChimeraWrapperImpl(const ChimeraWrapperImpl &);

		const ChimeraWrapperImpl & operator =(const ChimeraWrapperImpl &);

		bool _initialized;
		typedef std::shared_ptr<ChimeraHost> HostPtr;
		HostPtr _me;
		typedef std::shared_ptr<ChimeraState> StatePtr;
		StatePtr _state;

		inline static int encodeChimeraMessageType(message::MessageType type) {
			return CHIMERA_MESSAGE_TYPE_BASE + static_cast<int>(type);
		}

		inline static message::MessageType decodeChimeraMessagetype(int ctype) {
			return static_cast<message::MessageType>(ctype - CHIMERA_MESSAGE_TYPE_BASE);
		}

		void setUpcalls();
		void post_init();

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> createList(ChimeraHost **, int size) const;

		static ChimeraWrapperImpl * _impl;

		static void forward_upcall(Key **, Message **, ChimeraHost **);
		static void update_upcall(Key * k, ChimeraHost * h, int joined);
		static void deliver_upcall(Key * k, Message * m);

		const boost::posix_time::time_duration periodic_;
		const boost::posix_time::time_duration purge_distance_;

		typedef std::pair<boost::posix_time::ptime, net::NetworkType<net::CHIMERA>::Key> TimePair;
		typedef std::vector<TimePair> TimeList;
		typedef boost::unique_lock<boost::mutex> Guard;
		typedef std::map<net::NetworkType<net::CHIMERA>::Key, ChimeraHost *> KeyHostMap;

		TimeList neighbors_;
		mutable boost::mutex neighbors_mutex_;
		volatile bool purging_;
		boost::thread purger_;
		mutable boost::mutex map_mutex_;
		KeyHostMap map_;

		void processHeartbeat(const net::NetworkType<net::CHIMERA>::Key & from);
        void manageHeartbeats();
		void sendHeartbeat(const net::NetworkType<net::CHIMERA>::Key & to, bool ack = false);

	public:
		virtual ~ChimeraWrapperImpl() {
			purging_ = false;
			purger_.join();
		}

		void init(const int port);
		void init(const int port, const std::string & known_hostname, const int know_hostport);

		void send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg, typename net::NodeHandle<net::NetworkType<net::CHIMERA>>::Ptr_const hint);

		void send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg);

        net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr getSelfNodeHandle() const;

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> local_lookup(const net::NetworkType<net::CHIMERA>::Key & key, const int num, const bool safe) const;

		bool isResponsibleFor(const net::NetworkType<net::CHIMERA>::Key & key, const net::NetworkType<net::CHIMERA>::Key & node) const;

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> neighborSet(const int num) const;

		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> replicaSet(const net::NetworkType<net::CHIMERA>::Key & key, const int maxRank) const;

		bool range(const net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node, const int rank, const net::NetworkType<net::CHIMERA>::Key & lkey, const net::NetworkType<net::CHIMERA>::Key & rkey) const;

		void registerMessageType(const message::MessageType type, const bool ack) const;

		// MessagePtr_w createMessage(const int type, const Key& dest, const std::string& payload) const;
	};

} /* namespace chimera */
} /* namespace wrapper */
} /* namespace m2etis */

#endif /* __M2ETIS_WRAPPER_CHIMERA_CHIMERAWRAPPERIMPL_H__ */
