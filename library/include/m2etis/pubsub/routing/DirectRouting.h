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

/**
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_ROUTING_DIRECTROUTING_H__
#define __M2ETIS_PUBSUB_ROUTING_DIRECTROUTING_H__

/**
 * @class DirectRouting
 * @author Johannes Held
 */

#include <algorithm>
#include <string>
#include <vector>

#include "m2etis/message/MessageType.h"
#include "m2etis/message/info/routing/DirectRoutingInfo.h"
#include "m2etis/pubsub/PubSubSystemEnvironment.h"
#include "m2etis/pubsub/routing/BaseRouting.h"

#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace m2etis {
namespace pubsub {
namespace routing {

	template <class NetworkType>
	class DirectRouting : public BaseRouting<NetworkType> {
	public:
		typedef message::DirectRoutingInfo<NetworkType> RoutingInfoType;
		typedef std::vector<typename NetworkType::Key> KeyList;
		typedef std::pair<uint64_t, typename NetworkType::Key> TimePair;
		typedef std::vector<TimePair> TimeList;

		// [EOC] The next variables are more or less generic to all
		// distribution (algo) policies and describe their behavoir!

		// whether or not to register for subscribe messages in forward
		static const bool register_forward_subscribe = false;

		// whether or not to register for subscribe messages in deliver
		static const bool register_deliver_subscribe = true;

		// whether or not to register for unsubscribe messages in forward
		static const bool register_forward_unsubscribe = false;

		// whether or not to register for unsubscribe messages in deliver
		static const bool register_deliver_unsubscribe = true;
		// [EOC]

		// whether or not to activate an automatic periodic resent subscribtion
		static const bool periodicSubscribtion = true;

		// the time_duration between resent periodic subscribtions
		const uint64_t periodic_;

		// the time_duration to decide if a subscriber is purged form the list
		const uint64_t purge_distance_;

		PubSubSystemEnvironment * _pssi;

		// inform listener, whether a subscriber has been purged, e.g. by timeout
		// currently only needed for filter strategies
		boost::function<void(const typename NetworkType::Key)> removed_subscriber_eventlistener_;

		uint64_t purgeID_;

		DirectRouting(const unsigned short topic_name, PubSubSystemEnvironment * pssi, const typename NetworkType::Key & root) : BaseRouting<NetworkType>(topic_name, pssi), periodic_(direct::RESUBSCRIPTION_INTERVAL), purge_distance_(direct::PURGE_DISTANCE), _pssi(pssi), removed_subscriber_eventlistener_(), topic_name_(topic_name), self_(), selfsubscribed_(false), subscriber_(), purging_(true), _root(root) {
			purgeID_ = pssi->scheduler_.runRepeated(purge_distance_, boost::bind(&DirectRouting::purgeList, this), 6);
		}

		virtual ~DirectRouting() {
			purging_ = false;
			_pssi->scheduler_.stop(purgeID_);
		}

		void setSelf(const typename NetworkType::Key & self) {
			self_ = self;
		}

		void setUnsubscriptionListener(const boost::function<void(const typename NetworkType::Key)> & listener) {
			removed_subscriber_eventlistener_ = listener;
		}

		// set the self subscription status
		void selfSubscribed(const bool b) {
			selfsubscribed_ = b;
		}

		// get the self subscribtion status
		bool selfSubscribed() const {
			return selfsubscribed_;
		}

		void configureRoutingInfo(message::ActionType & msgType, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key &) {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			switch (msgType) {
				case message::SUBSCRIBE: {
					msgType = message::SUBSCRIBE;
					break;
				}
				case message::UNSUBSCRIBE: {
					msgType = message::UNSUBSCRIBE;
					break;
				}
				case message::PUBLISH: {
					msgType = message::PUBLISH;
					break;
				}
				case message::NOTIFY: {
					msgType = message::NOTIFY;
					break;
				}
				case message::CONTROL: {
					msgType = message::CONTROL;
					break;
				}
				default:
					M2ETIS_LOG_ERROR("Direct Routing", "configureRoutingInfo called with wrong action type");
			}
			return;
		}

		/**
		 * Returns a list of target nodes for the specified message type and algoinfo in regard to the node's
		 * position in the logic distribution tree.
		 *
		 * @param mtype the type of the message (SUBSCRIBE, UNSUBSCIBE, or PUBLISH)
		 * @param algoinfo only contains info if mytpe == PUBLISH
		 */
		KeyList getTargetNodes(const message::ActionType mtype, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key &) const {
			/*
			 * All messages must be sent to root.
			 * Even if it's a publishmessage and root is subscribed, too.
			 * In deliver::publish, root will take care to deliver the message and spread it to the subscribers.
			 */
			KeyList m;
			if (mtype == message::SUBSCRIBE || mtype == message::UNSUBSCRIBE) {
				// If the current node is root, then we don't need to send a message to ourself!
				if (self_ == _root) {
					return m;
				}
				m.push_back(_root);
			} else if (mtype == message::PUBLISH) {
				m.push_back(_root);
			} else if (mtype == message::NOTIFY || mtype == message::CONTROL) {
				struct T {
					static typename NetworkType::Key get_from_pair(const TimePair & p) {
						return p.second;
					}
				};

				m.resize(subscriber_.size());
				std::transform(subscriber_.begin(), subscriber_.end(), m.begin(), T::get_from_pair);
			} else {
				assert(false);
			}

			return m;
		}

		/**
		 * Handle a subscribe message
		 *
		 * @param sender the subscriber's key
		 * @param algoinfo the payload created by this algorithm at the subscriber's node
		 * @return an information unit, whether the message should be stopped or needs to be changed
		 */
		bool processSubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key &, message::ActionType &) override {
			if (sender != self_) { // I don't send a message to me if I'm root. But somehow the variable inside R changed!
				bool found = false;
				typename TimeList::iterator iter;
				for (iter = subscriber_.begin(); iter != subscriber_.end(); ++iter) {
					if (iter->second == sender) {
						found = true;
						break;
					}
				}
				if (found) {
					// update entry
					iter->first = _pssi->clock_.getTime();
				} else {
					// insert entry
					subscriber_.push_back(std::make_pair(_pssi->clock_.getTime(), sender));
				}
			}
			routingInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			return true;
		}

		/**
		 * Handle a unsubscribe message
		 *
		 * @param sender the unsubscriber's key
		 * @param algoinfo the payload created by this algorithm at the unsubscriber's node
		 * @return an information unit, whether the message should be stopped or needs to be changed
		 */
		void processUnsubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key &, message::ActionType &) {
			struct T {
				static bool test(const typename NetworkType::Key & send, const TimePair & paar) {
					return paar.second == send;
				}
			};

			subscriber_.erase(std::remove_if(subscriber_.begin(), subscriber_.end(), boost::bind(T::test, sender, _1)), subscriber_.end());

			return;
		}

		/**
		 * Handle a publish message
		 *
		 * @param sender the publisher's key
		 * @param algoinfo the payload created by this algorithm at the publisher's node
		 * @return an information unit, whether the message should be spread out to subscribers ond if the
		 * current node is the root-node for that topic.
		 */
		void processPublishPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key &, typename NetworkType::Key &, message::ActionType & msgType) {
			assert(self_ == _root);
			if (self_ == _root) {
				if (!subscriber_.empty()) {
					msgType = message::NOTIFY;
					return;
				}
			}

			routingInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			return;
		}

		/**
		 * Handle a notify message
		 *
		 * @param sender the notifier's key
		 * @param algoinfo the payload created by this algorithm at the notify's node
		 * @return an information unit, whether the message should be spread out to subscribers ond if the
		 * current node is the root-node for that topic.
		 */
		void processNotifyPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key &, typename NetworkType::Key &, message::ActionType &) {
			routingInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			return;
		}

		void processControlPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key &, typename NetworkType::Key &, message::ActionType &) {
			return;
		}

	private:
		// The topic name
		const unsigned short topic_name_;
		// The own key of this actual node
		typename NetworkType::Key self_;
		// Is this node subscribed on the topic?
		bool selfsubscribed_;

		// List of all subscribers (TimeStamp and Key)
		TimeList subscriber_;

		// Control variables for the purging thread
		mutable boost::mutex subscriber_mutex_;
		volatile bool purging_;

		typename NetworkType::Key _root;

		typename RoutingInfoType::Ptr cast(typename message::RoutingInfo<NetworkType>::Ptr ptr) const {
			typename RoutingInfoType::Ptr ret = boost::dynamic_pointer_cast<RoutingInfoType>(ptr);
			if (!ret) {
				M2ETIS_LOG_ERROR("Routing Strategy", "Downcast error of routingInfo");
			}
			return ret;
		}

		bool purgeList() {
			// Helper-Struct to create a specialized method which can be used to generically erase from a vector
			struct T {
				static bool test(const uint64_t & p, const uint64_t & jetzt, const TimePair & paar) {
					if ((jetzt - paar.first) > p) {
						return true;
					}
					return false;
				}
			};

			if (subscriber_.empty()) {
				return purging_;
			}

			// lock the list
			// get the current timestamp
			// and remove every subscriber which hasn't resubscribed within the "purge_distance"
			uint64_t jetzt = _pssi->clock_.getTime();

			auto iter_first_erased_subscriber = std::remove_if(subscriber_.begin(), subscriber_.end(), boost::bind(T::test, purge_distance_, jetzt, _1));
			// notify listeners about erased subscribers:
			for (auto iter_subscriber = iter_first_erased_subscriber; iter_subscriber != subscriber_.end(); ++iter_subscriber) {
				removed_subscriber_eventlistener_(iter_subscriber->second);
			}

			subscriber_.erase(iter_first_erased_subscriber, subscriber_.end());

			return purging_;
		}
	};

} /* namespace routing */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ROUTING_DIRECTROUTING_H__ */

/**
 *  @}
 */
