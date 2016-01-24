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
 * @{
 */

#ifndef __M2ETIS_PUBSUB_ROUTING_HIERARCHICALSPREADITROUTING_H__
#define __M2ETIS_PUBSUB_ROUTING_HIERARCHICALSPREADITROUTING_H__

/**
 * @class HierarchicalSpreaditRouting
 * @author Daniel Bonrath
 */

#include <algorithm>
#include <string>
#include <vector>

#include "m2etis/message/MessageType.h"
#include "m2etis/message/info/routing/HierarchicalSpreadItRoutingInfo.h"

#include "m2etis/pubsub/PubSubSystemEnvironment.h"
#include "m2etis/pubsub/routing/BaseRouting.h"

#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace m2etis {
namespace pubsub {
namespace routing {

	template<class NetworkType, unsigned int ChildAmount>
	class HierarchicalSpreaditRouting : public BaseRouting<NetworkType> {
	public:
		typedef message::HierarchicalSpreadItRoutingInfo<NetworkType> RoutingInfoType;
		typedef std::pair<long, typename NetworkType::Key> TimePair;
		typedef std::vector<typename NetworkType::Key> KeyList;
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
		const long periodic_;

		// the time_duration to decide if a subscriber is purged form the list
		const long purge_distance_;

		PubSubSystemEnvironment * _pssi;

		// inform listener, whether a subscriber has been purged, e.g. by timeout
		// currently only needed for filter strategies
		boost::function<void(const typename NetworkType::Key)> removed_subscriber_eventlistener_;

		unsigned int purgeID_;

		HierarchicalSpreaditRouting(unsigned int topic_name, PubSubSystemEnvironment * pssi, const typename NetworkType::Key & root) :
			BaseRouting<NetworkType>(topic_name, pssi)
			, periodic_(hierarchicalspreadit::RESUBSCRIPTION_INTERVAL), purge_distance_(hierarchicalspreadit::PURGE_DISTANCE), _pssi(pssi)
			, removed_subscriber_eventlistener_()
			, topic_name_(topic_name), self_(), subscribed_(false), subscriber_()
			, purging_(true), _root(root) {
			purgeID_ = pssi->scheduler_.runRepeated(purge_distance_, boost::bind(&HierarchicalSpreaditRouting::purgeList, this), 0);
		}

		virtual ~HierarchicalSpreaditRouting() {
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
				if (!parent_vector.empty()) {
					rInfo->node_adress = parent_vector.at(0).second;
				}
				break;
			}
			case message::PUBLISH: {
				msgType = message::NOTIFY;
				rInfo->selfSend = true;
				rInfo->sender = self_;
				break;
			}
			case message::NOTIFY: {
				msgType = message::NOTIFY;
				rInfo->sender = self_;
				break;
			}
			case message::CONTROL: {
				msgType = message::CONTROL;
				break;
			}
			default:
				M2ETIS_THROW_FAILURE("HierarchicalSpreaditRouting", "configureRoutingInfo called with wrong action type", -1);
			}
			return;
		}

		KeyList getTargetNodes(const message::ActionType mtype, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key & receiver) const {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			/*
			 * All messages must be sent to root.
			 * Even if it's a publishmessage and root is subscribed, too.
			 * In deliver::publish, root will take care to deliver the message and spread it to the subscribers.
			 */
			KeyList m;
			if (mtype == message::SUBSCRIBE) {
				// I've been redirected to a new parent
				if (rInfo->action == message::RoutingInfo<NetworkType>::RoutingType::REDIRECT) {
					m.push_back(rInfo->node_adress);
				} else if (!parent_vector.empty()) { // if I already have a parent, send subscribe to him - works like a heart beat
					m.push_back(parent_vector.begin()->second);
				} else { // subscription process starts from the root
					if (self_ == _root) {
						return m;
					} else {
						m.push_back(_root);
					}
				}
			} else if (mtype == message::CONTROL) {
				// receiver has been saved earlier
				if (receiver == typename NetworkType::Key()) {
					struct T {
						 static typename NetworkType::Key get_from_pair(const HierarchicalSpreaditRouting::TimePair & p) {
							return p.second;
						 }
					};

					m.resize(subscriber_.size());
					std::transform(subscriber_.begin(), subscriber_.end(), m.begin(), T::get_from_pair);
				} else {
					m.push_back(receiver);
				}
			} else if (mtype == message::PUBLISH) {
				// here we need to send the message always to root (even if we are the root)
				// because the processing for publish messages is done in deliver
				assert(false);
			} else if (mtype == message::NOTIFY) {
				if (!parent_vector.empty()) { // if node has parent
					if (rInfo->sender != parent_vector.begin()->second) { // and the messages doesn't come from the parent
						m.push_back(parent_vector.begin()->second); // the message has to be sent to the parent
					}
				} else { // the node doesn't have a parent
					if (self_ != _root) { // this node isn't root node
						m.push_back(_root); // the message has to be sent to root
					}
				}

				for (TimePair p : subscriber_) {
					if (p.second != rInfo->sender) {
						m.push_back(p.second); // add every subscriber not being the sender of the message as receiver
					}
				}
				if (rInfo->selfSend) {
					m.push_back(self_);
				}
				rInfo->sender = self_;
			} else if (mtype == message::UNSUBSCRIBE) {
				// message must be published, that means from root to subscribers
				// Simply return a list of subscribed nodes

				// That's nicer than for_each and push_back, isn't it?
				// std::transform is really good!
				struct T {
					 static typename NetworkType::Key get_from_pair(const TimePair & p) {
						return p.second;
					 }
				};

				m.resize(subscriber_.size());
				std::transform(subscriber_.begin(), subscriber_.end(), m.begin(), T::get_from_pair);

				if (!parent_vector.empty()) {
					m.push_back(parent_vector.at(0).second);
				}
			} else {
				M2ETIS_THROW_FAILURE("Spreadit Routing", "unknown action type", -1);
			}

			return m;
		}

		bool processSubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) override {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);

			bool is_subscribe_successful = true;
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
				// send ack to my subscriber - heart beat
				rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::ACK;
			} else {
				// check if I have capacity for more children
				if (bandwidth_capacity > subscriber_.size()) {
					// insert entry
					subscriber_.push_back(std::make_pair(_pssi->clock_.getTime(), sender));
					M2ETIS_LOG_DEBUG(self_.toStr(), " added " << sender.toStr());
					rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::ACK;
					msgType = message::CONTROL;
				} else { // redirect to another node
					rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::REDIRECT;
					rInfo->node_adress = typename NetworkType::Key(subscriber_.at(round_robin_pointer).second);
					round_robin_pointer = (round_robin_pointer + 1) % subscriber_.size();
					msgType = message::CONTROL;
					is_subscribe_successful = false;
				}
			}

			receiver = sender;
			return is_subscribe_successful;
		}

		void processUnsubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);

			struct T {
				static bool test(const typename NetworkType::Key & send, const TimePair & paar) {
					return paar.second == send;
				}
			};

			subscriber_.erase(std::remove_if(subscriber_.begin(), subscriber_.end(), boost::bind(T::test, sender, _1)), subscriber_.end());

			// if it was my parent, he has sent a redirect address
			if (!parent_vector.empty()) {
				if (sender == parent_vector.at(0).second) {
					msgType = message::SUBSCRIBE;
					receiver = rInfo->node_adress;
					rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::REDIRECT;
				}
			}
			rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
		}

		void processPublishPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			assert(false);
		}

		void processNotifyPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			if ((subscriber_.empty() && (parent_vector.empty() && self_ == _root)) || sender == self_) {
				routingInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			}
		}

		void processControlPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key & receiver, message::ActionType & msgType) {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			// new parent acknowledges the subscription
			if (rInfo->action == message::RoutingInfo<NetworkType>::RoutingType::ACK) {
				if (parent_vector.empty()) {
					parent_vector.push_back(std::make_pair(_pssi->clock_.getTime(), sender));
				} else {
					parent_vector.begin()->second = sender;
					parent_vector.begin()->first = _pssi->clock_.getTime();
				}
				rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			} else if (rInfo->action == message::RoutingInfo<NetworkType>::RoutingType::REDIRECT) { // parent forwards my subscription request to another node
				msgType = message::SUBSCRIBE;
				receiver = rInfo->node_adress;
				rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::REDIRECT;
			} else { // nothing to do, stop the workflow
				rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			}
		}

	private:
		// The topic name
		const unsigned int topic_name_;
		// The own key of this actual node
		typename NetworkType::Key self_;
		// Is this node subscribed on the topic?
		bool subscribed_;

		unsigned int bandwidth_capacity = ChildAmount;
		unsigned int round_robin_pointer = 0;

		// List of all subscribers (TimeStamp and Key)
		TimeList subscriber_;
		// List of all parents (TimeStamp and Key)
		TimeList parent_vector;

		// Control variables for the purging thread
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
				static bool test(const long & p, const long & jetzt, const TimePair & paar) {
					if ((jetzt - paar.first) > p) {
						return true;
					}
					return false;
				}
			};

			if (subscriber_.empty() && parent_vector.empty()) {
				return purging_;
			}

			// lock the list
			// get the acutal timestamp
			// and remove every subscriber which hasn't resubscribed within the "purge_distance"
			long jetzt = _pssi->clock_.getTime();

             // also: inform unsubscription listeners:
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

#endif /* __M2ETIS_PUBSUB_ROUTING_HIERARCHICALSPREADITROUTING_H__ */

/**
 *  @}
 */
