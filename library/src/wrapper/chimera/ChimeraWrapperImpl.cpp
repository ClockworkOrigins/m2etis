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

#include <algorithm>
#include <stdexcept>

#include "m2etis/message/MessageFactory.h"
#include "m2etis/net/NodeHandle.h"
#include "m2etis/wrapper/chimera/ChimeraWrapperImpl.h"

#include "boost/foreach.hpp"

extern "C" {
	#include "chimera/route.h"
	// I know that it's defined in message.c
	void message_free(Message *);
}

namespace m2etis {
namespace wrapper {
namespace chimera {

	ChimeraWrapperImpl * ChimeraWrapperImpl::_impl = nullptr;

	void ChimeraWrapperImpl::post_init() {
		_initialized = true;
		// setting me
		memcpy(_me.get(), static_cast<ChimeraGlobal *>(_state.get()->chimera)->me, sizeof(ChimeraHost));

		chimera_register(_state.get(), encodeChimeraMessageType(message::STOP_MSG_NR), 1);
		chimera_register(_state.get(), encodeChimeraMessageType(message::HEARTBEAT_MSG_NR), 1);
		chimera_register(_state.get(), encodeChimeraMessageType(message::HEARTBEAT_ACK_NR), 1);

		// if the callback is already set - set our upcalls, too!
		if (_cb) {
			setUpcalls();
		}
	}

	void ChimeraWrapperImpl::init(const int port) {
		_state = StatePtr(chimera_init(port));
		chimera_join(_state.get(), 0);
		post_init();
	}

	void ChimeraWrapperImpl::init(int port, const std::string & known_hostname, int known_hostport) {
		_state = StatePtr(chimera_init(port));
		ChimeraHost * joinHost = 0;
		if (known_hostport != -1 && known_hostname != "") {
			joinHost = host_get(_state.get(), const_cast<char *>(known_hostname.c_str()), known_hostport);
		}

		chimera_join(_state.get(), joinHost);
		post_init();
	}

	void ChimeraWrapperImpl::setUpcalls() {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}
		chimera_deliver(_state.get(), deliver_upcall);
		chimera_update(_state.get(), update_upcall);
		chimera_forward(_state.get(), forward_upcall);
	}

	void ChimeraWrapperImpl::update_upcall(Key * k, ChimeraHost * h, int joined) {
		if (_impl->_cb == 0) {
			return;
		}

		net::NetworkType<net::CHIMERA>::Key key(std::string(get_key_string(k)));

		net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node(new net::NodeHandle<net::NetworkType<net::CHIMERA> >(net::NetworkType<net::CHIMERA>::Key(std::string(get_key_string(&(h->key)))), std::string(h->name), h->port));

		_impl->_cb->update(key, node, joined);
	}

	void ChimeraWrapperImpl::deliver_upcall(Key * k, Message * m) {
		if (_impl->_cb == 0) {
			return;
		}

		message::MessageType decoded_type = decodeChimeraMessagetype(m->type);
		/* If a HEARTBEAT_ACK is received, let's process it.
		 * A HEARTBEAT_MSG must be acknowledged.
		 */
		if (decoded_type == message::HEARTBEAT_ACK_NR) {
			// Yes. It must be that complicated …
			message::InternalMessage<net::NetworkType<net::CHIMERA>> msg(m->payload, m->size);
			_impl->processHeartbeat(msg.getPayload());
			return;
		}
		if (decoded_type == message::HEARTBEAT_MSG_NR) {
			_impl->sendHeartbeat(net::NetworkType<net::CHIMERA>::Key(std::string(m->payload)), true);
			return;
		}

		std::string key = std::string(get_key_string(k));
		InternalMessage msg(decoded_type, std::string(get_key_string(&(m->dest))), m->payload, m->size);
		// msg->receiver_ = std::string(get_key_string(&(m->dest)));
		// MessageTHelper::transform(msg->payload_, m->payload, m->size);
		// msg->type_ = m->type;

		_impl->_cb->deliver(key, msg);
	}

	void ChimeraWrapperImpl::forward_upcall(Key ** k, Message ** m, ChimeraHost ** h) {
		if (_impl->_cb == 0) {
			return;
		}

		// First we transform chimera's type into our simpler types
		// Next we push the message up to our user
		// Afterwards we bring his changes back to chimera
		Message * mp = *m;
		Key dest = mp->dest;
		std::string key = std::string(get_key_string(*k));
		InternalMessage msg(decodeChimeraMessagetype((*m)->type), std::string(get_key_string(&dest)), (*m)->payload, (*m)->size);

		// msg->receiver_ = std::string(get_key_string(&dest));
		// MessageTHelper::transform(msg->payload_, mp->payload, mp->size);
		// msg->type_ = (*m)->type;

		net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node(new net::NodeHandle<net::NetworkType<net::CHIMERA> >(net::NetworkType<net::CHIMERA>::Key(std::string(get_key_string(&((*h)->key)))), std::string((*h)->name), (*h)->port));

		// Call thy user
		bool stop = _impl->_cb->forward(key, msg, node);

		if (stop) {
			// Sending the message to myself, with a type nobody wants to know.
			key_assign(*k, _impl->_me->key);
			mp->dest = _impl->_me->key;
			mp->type = encodeChimeraMessageType(message::STOP_MSG_NR);
		} else {
			// Now we need to give the users changes back to the network.
			Key neu;

			// Test if the key (k) has changed
			str_to_key(const_cast<char *>(key.c_str()), &neu);
			if (key_equal(**k, neu) == 1) {
				key_assign(*k, neu);
			}

			str_to_key(const_cast<char *>(msg.getReceiver().c_str()), &neu);

			key_assign(&(mp->dest), neu);
			mp->type = static_cast<int>(msg.getType());
			free(mp->payload);
			mp->size =  msg.getPayloadLength();
			mp->payload = (char *) malloc(sizeof(char) * size_t(mp->size));
			std::copy(msg.getPayloadRef().begin(), msg.getPayloadRef().end(), mp->payload);

			// FIXME What's about the host?
		}
	}

	void ChimeraWrapperImpl::registerMessageType(message::MessageType type, bool ack) const {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}

		int a = 1;
		if (ack == false) {
			a = 2;
		}
		chimera_register(_state.get(), encodeChimeraMessageType(type), a);
	}

	void ChimeraWrapperImpl::send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg) {
		net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr x;
		send(msg, x);
	}

	void ChimeraWrapperImpl::send(const typename message::NetworkMessage<net::NetworkType<net::CHIMERA>>::Ptr msg, typename net::NodeHandle<net::NetworkType<net::CHIMERA>>::Ptr_const hint) {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}

		Key chim_key;
		str_to_key(const_cast<char *>(msg->receiver.toStr().c_str()), &chim_key);
		Message * msg2 = message_create(chim_key, encodeChimeraMessageType(msg->type), payload.getPayloadLength(), const_cast<char *>(payload.getPayloadChar()));

		ChimeraHost * chim_host = 0;

		if (hint.get() != nullptr) {
			chim_host = host_get(_state.get(), const_cast<char *>(hint->hostname_.c_str()), hint->port_);
		}

		chimera_route(_state.get(), &chim_key, msg2, chim_host);
		message_free(msg2);
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapperImpl::local_lookup(const net::NetworkType<net::CHIMERA>::Key & key, const int num, const bool safe) const {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}

		Key chim_key;
		str_to_key(const_cast<char *>(key.c_str()), &chim_key);
		ChimeraHost ** nachbarn = route_lookup(_state.get(), chim_key, num, safe);

		return createList(nachbarn, num);
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapperImpl::neighborSet(const int num)  const {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}

		ChimeraHost ** nachbarn = route_neighbors(_state.get(), num);

		return createList(nachbarn, num);
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapperImpl::replicaSet(const net::NetworkType<net::CHIMERA>::Key & key, const int maxRank) const {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}
		M2ETIS_THROW_API("ChimeraWrapperImpl", "replicaSet not yet implemented");
	}

	bool ChimeraWrapperImpl::range(const net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node, const int rank, const net::NetworkType<net::CHIMERA>::Key & lkey, const net::NetworkType<net::CHIMERA>::Key & rkey) const {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}
		M2ETIS_THROW_API("ChimeraWrapperImpl", "range not yet implemented");
	}

	net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr ChimeraWrapperImpl::getSelfNodeHandle() const {
		if (!_initialized) {
			M2ETIS_THROW_API("ChimeraWrapperImpl", "not initialized. call init first!");
		}

		net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node(new net::NodeHandle<net::NetworkType<net::CHIMERA> >(net::NetworkType<net::CHIMERA>::Key(std::string(get_key_string(&(_me->key)))), std::string(_me->name), _me->port));

		return node;
	}

	bool ChimeraWrapperImpl::isResponsibleFor(const net::NetworkType<net::CHIMERA>::Key & key, const net::NetworkType<net::CHIMERA>::Key & node) const {
		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> liste = local_lookup(key, 1, false);
		if (liste.size() == 1 && liste[0]->key_ == node) {
			return true;
		}
		return false;
	}

	std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> ChimeraWrapperImpl::createList(ChimeraHost ** liste, const int size) const {
		std::list<net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr> nachbars_liste;
		if (liste != 0) {
			for(int i = 0; i < size; ++i) {
				if (!liste[i]) { // if one item is 0 there are no more valid entries.
					return nachbars_liste;
				}

				// In der übergebenen Liste sind manche Eintröge öfters enthalten
				// diese filtere ich raus.
				bool next = false;
				for (int j = 0; j < i; ++j) {
					if (liste[i] == liste[j]) {
						next = true;
						break;
					}
				}
				if (next) {
					continue;
				}
				net::NodeHandle<net::NetworkType<net::CHIMERA> >::Ptr node(new net::NodeHandle<net::NetworkType<net::CHIMERA> >(net::NetworkType<net::CHIMERA>::Key(get_key_string(&(liste[i]->key))), std::string(liste[i]->name), liste[i]->port));

				nachbars_liste.push_back(node);
			}
		}
		return nachbars_liste;
	}


	/**
	 * @brief Remove the given key from the list "awaiting heartbeat ack".
	 * @param key the key to remove
	 */
	void ChimeraWrapperImpl::processHeartbeat(const net::NetworkType<net::CHIMERA>::Key & key) {
		struct T {
			static bool test(const net::NetworkType<net::CHIMERA>::Key & sender, const TimePair & paar) {
				return paar.second == sender;
			}
		};

		{
			// Remove the key from the list.
			Guard lock(neighbors_mutex_);
			TimeList::iterator iter = std::find_if(neighbors_.begin(), neighbors_.end(), boost::bind(T::test, key, _1));
			if (iter == neighbors_.end()) {
				return;
			}
			neighbors_.erase(iter); // don't delete the END. It'll bail out!
		}
		{
			// Remove the key from the map.
			Guard lock(map_mutex_);
			KeyHostMap::iterator ifind = map_.find(key);
			if (ifind != map_.end()) {
				delete ifind->second;
				map_.erase(ifind);
			}
		}
	}

	void ChimeraWrapperImpl::manageHeartbeats() {
		struct T {
			static bool test(const boost::posix_time::time_duration & p, const boost::posix_time::ptime & jetzt, const TimePair & paar) {
				return (jetzt - paar.first) > p;
			}
			static bool find(const net::NetworkType<net::CHIMERA>::Key & sender, const TimePair & paar) {
				return paar.second == sender;
			}
		};
		while (purging_) {
			boost::this_thread::sleep(periodic_);

			/* For each entry in routingtable
			 * we send a heartbeat-message
			 * and add that entry with a timestamp into our list.
			 */
			ChimeraHost ** rtable = route_get_table(_state.get());
			int i = 0;

			while (rtable[i] != NULL) {
				net::NetworkType<net::CHIMERA>::Key node(get_key_string(&(rtable[i]->key)));
				TimeList::iterator iter = std::find_if(neighbors_.begin(), neighbors_.end(), boost::bind(T::find, node, _1));

				// only send a heartbeat if we're not waiting for an ack.
				if (iter == neighbors_.end()) {
					sendHeartbeat(node);
					{
						Guard lock(neighbors_mutex_);
						neighbors_.push_back(std::make_pair(boost::posix_time::ptime(boost::posix_time::microsec_clock::local_time()), node));
					}

					ChimeraHost * h = new ChimeraHost();
					memcpy(h, rtable[i], sizeof(ChimeraHost));
					// FIXME Yeah I know, that I'm missing the name. But I don't need the name..
					{
						Guard lock(map_mutex_);
						map_.insert(std::make_pair(node, h));
					}
				} else {
					sendHeartbeat(node);
				}

				++i;
			}

			// and do the cleanup
			i = 0;
			while (rtable[i] != NULL) {
				host_release(_state.get(), rtable[0]);
				++i;
			}
			free(rtable);

			{
				Guard lock(neighbors_mutex_);
				Guard lock2(map_mutex_);

				boost::posix_time::ptime jetzt(boost::posix_time::microsec_clock::local_time());
				TimeList::iterator suche = std::find_if(neighbors_.begin(), neighbors_.end(), boost::bind(T::test, purge_distance_, jetzt, _1));
				while (suche != neighbors_.end()) {
					KeyHostMap::iterator ifind = map_.find(suche->second);
					if (ifind != map_.end()) {
						route_update(_state.get(), ifind->second, false);
						delete ifind->second;
						map_.erase(ifind);

						neighbors_.erase(suche);
					}
					suche = std::find_if(neighbors_.begin(), neighbors_.end(), boost::bind(T::test, purge_distance_, jetzt, _1));
				}
			}

			rtable = route_get_table(_state.get());
			i = 0;
			while (rtable[i] != NULL) {
				std::string node(get_key_string(&(rtable[i]->key)));
				++i;
			}
			// and do the cleanup
			i = 0;
			while (rtable[i] != NULL) {
				host_release(_state.get(), rtable[0]);
				++i;
			}
			free(rtable);
		}
	}

	void ChimeraWrapperImpl::sendHeartbeat(const net::NetworkType<net::CHIMERA>::Key & to, bool ack) {
		std::string from(get_key_string(&(_me->key)));
		InternalMessage msg(from);
		if (!ack) {
			send(message::HEARTBEAT_MSG_NR, to, msg);
		} else {
			send(message::HEARTBEAT_ACK_NR, to, msg);
		}
	}

} /* namespace chimera */
} /* namespace wrapper */
} /* namespace m2etis */
