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
 * \addtogroup pubsub
 * @ {
 */

#ifndef __M2ETIS_PUBSUB_ROUTING_SCRIBEROUTING_H__
#define __M2ETIS_PUBSUB_ROUTING_SCRIBEROUTING_H__

/**
 * @class ScribeRouting
 * @author Hristiyan Pehlivanov
 */

#include <algorithm>
#include <string>
#include <vector>

#include "m2etis/message/MessageType.h"
#include "m2etis/message/info/routing/ScribeRoutingInfo.h"

#include "m2etis/pubsub/PubSubSystemEnvironment.h"

#include "boost/thread.hpp"

namespace m2etis {
namespace pubsub {
namespace routing {

	template<class NetworkType>
	class ScribeRouting : public BaseRouting<NetworkType>  {
	public:
		typedef message::ScribeRoutingInfo<NetworkType> RoutingInfoType;
		typedef std::vector<typename NetworkType::Key> KeyList;
		typedef std::pair<uint64_t, typename NetworkType::Key> TimePair;
		typedef std::vector<TimePair> TimeList;

		// [EOC] The next variables are more or less generic to all
		// routing policies and describe their behavior!

		// whether or not to register for subscribe messages in forward
		static const bool register_forward_subscribe = true;

		// whether or not to register for subscribe messages in deliver
		static const bool register_deliver_subscribe = false;

		// whether or not to register for unsubscribe messages in forward
		static const bool register_forward_unsubscribe = true;

		// whether or not to register for unsubscribe messages in deliver
		static const bool register_deliver_unsubscribe = false;
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

		ScribeRouting(const uint16_t topic_name, PubSubSystemEnvironment * pssi, const typename NetworkType::Key & root) : BaseRouting<NetworkType>(topic_name, pssi)
			, periodic_(scribe::RESUBSCRIPTION_INTERVAL), purge_distance_(scribe::PURGE_DISTANCE), _pssi(pssi)
			, topic_name_(topic_name), self_(), subscribed_(false), subscriber_()
			, purging_(true), _root(root) {
			purgeID_ = pssi->scheduler_.runRepeated(purge_distance_, boost::bind(&ScribeRouting::purgeList, this), 0);
		}

		virtual ~ScribeRouting() {
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
			subscribed_ = b;
		}

		// get the self subscribtion status
		bool selfSubscribed() const {
			return subscribed_;
		}

		void configureRoutingInfo(message::ActionType & msgType, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key & receiver) {
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
				default: {
					M2ETIS_LOG_ERROR("Scribe Routing", "configureRoutingInfo called with wrong action type");
				}
			}
		}

		KeyList getTargetNodes(const message::ActionType mtype, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key & receiver) const {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);

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
				// TODO: (???) use the IP address if it is known, publish messages are sent to the root who starts the distribution
				// if root is not known, use pastry routing to localize it
				m.push_back(_root);
			} else {
				// message must be published, that means from root to subscribers
				// Simply return a list of subscribed nodes

				// That's nicer than for_each and push_back, isn't it?
				// std::transform is really good!
				struct T {
					 static typename NetworkType::Key get_from_pair(const ScribeRouting::TimePair & p) {
						return p.second;
					 }
				};
				m.resize(subscriber_.size());
				std::transform(subscriber_.begin(), subscriber_.end(), m.begin(), T::get_from_pair);
			}
			return m;
		}

		bool processSubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			if (sender == self_) { // TODO: (Daniel) hack, not sure that's right, but otherwise nodes subscribe on itself
				return false;
			}
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
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
			// stop the subscribe message
			if (selfSubscribed()) {
				rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			}
			return true;
		}

		void processUnsubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			// TODO: (???) use subscribed_ to check if I am a subscriber or just forwarder
			struct T {
				static bool test(const typename NetworkType::Key & send, const TimePair & paar) {
					return paar.second == send;
				}
			};

			subscriber_.erase(std::remove_if(subscriber_.begin(), subscriber_.end(), boost::bind(T::test, sender, _1)), subscriber_.end());
		}

		/**
		 * Handle a publish message
		 */
		void processPublishPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			M2ETIS_LOG_INFO("Received Publish msg", "");
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);

			msgType = message::NOTIFY;
		}

		void processNotifyPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
		}

		void processControlPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			return;
		}

	private:
		// The topic name
		const uint16_t topic_name_;
		// The own key of this actual node
		typename NetworkType::Key self_;
		// Is this node subscribed on the topic?
		bool subscribed_;

		// List of all subscribers (TimeStamp and Key)
		TimeList subscriber_;

		// Control variables for the purging thread
		boost::thread purger_;
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
				static bool test(const uint64_t & p, const uint64_t & jetzt, const ScribeRouting::TimePair & paar) {
					if ((jetzt - paar.first) > p) {
						return true;
					}
					return false;
				}
			};

			if (subscriber_.empty()) {
				return true;
			}

			// lock the list
			// get the acutal timestamp
			// and remove every subscriber which hasn't resubscribed within the "purge_distance"
			uint64_t jetzt = _pssi->clock_.getTime();

			auto iter_first_erased_subscriber = std::remove_if(subscriber_.begin(), subscriber_.end(), boost::bind(T::test, purge_distance_, jetzt, _1));
			// notify listeners about erased subscribers:
			for (auto iter_subscriber = iter_first_erased_subscriber; iter_subscriber != subscriber_.end(); ++iter_subscriber) {
				removed_subscriber_eventlistener_(iter_subscriber->second);
			}

			subscriber_.erase(iter_first_erased_subscriber, subscriber_.end());

			return true;
		}
	};

} /* namespace routing */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ROUTING_SCRIBEROUTING_H__ */

/**
 *  @}
 */
