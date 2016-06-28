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

#ifndef __M2ETIS_PUBSUB_ROUTING_DIRECTBROADCASTROUTING_H__
#define __M2ETIS_PUBSUB_ROUTING_DIRECTBROADCASTROUTING_H__

#include <algorithm>
#include <string>
#include <vector>

#include "m2etis/message/MessageType.h"
#include "m2etis/message/info/routing/DirectBroadcastRoutingInfo.h"
#include "m2etis/pubsub/PubSubSystemEnvironment.h"
#include "m2etis/pubsub/routing/BaseRouting.h"

namespace m2etis {
namespace pubsub {
namespace routing {

	/**
	 * \brief creates connections between every single node and sends messages directly, so every node is a root node for its connections
	 * doesn't work with GMSOrder because it doesn't use publish messages but notifies directly
	 */
	template<class NetworkType>
	class DirectBroadcastRouting : public BaseRouting<NetworkType> {
	public:
		typedef message::DirectBroadcastRoutingInfo<NetworkType> RoutingInfoType;
		typedef std::vector<typename NetworkType::Key> KeyList;
		typedef std::set<typename NetworkType::Key> KeySet;
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
		std::function<void(const typename NetworkType::Key)> _removed_subscribereventlistener;

		uint64_t purgeID_;

		uint64_t registerID_;

		DirectBroadcastRouting(const unsigned short topic_name, PubSubSystemEnvironment * pssi, const typename NetworkType::Key & root) : BaseRouting<NetworkType>(topic_name, pssi), periodic_(directbroadcast::RESUBSCRIPTION_INTERVAL), purge_distance_(directbroadcast::PURGE_DISTANCE), _pssi(pssi), _removed_subscribereventlistener(), purgeID_(), registerID_(), topic_name_(topic_name), self_(), selfSubscribed_(false), _subscriber(), _purging(true), _newSubs(), _nodes(), _root(root) {
			purgeID_ = pssi->scheduler_.runRepeated(purge_distance_, std::bind(&DirectBroadcastRouting::purgeList, this), 6);
			registerID_ = pssi->scheduler_.runOnce(1, [this]() {
				registerOnRoot();
				return false;
			}, 1);
		}

		virtual ~DirectBroadcastRouting() {
			_purging = false;
			_pssi->scheduler_.stop(purgeID_);
			_pssi->scheduler_.stop(registerID_);
		}

		void registerOnRoot() {
			if (self_ != _root) {
#if I6E_PLATFORM == I6E_PLATFORM_WIN32
				RoutingInfoType::Ptr newInfo = boost::make_shared<RoutingInfoType>();
#elif I6E_PLATFORM == I6E_PLATFORM_LINUX
				typename RoutingInfoType::Ptr newInfo = boost::make_shared<RoutingInfoType>();
#endif
				newInfo->action = RoutingInfoType::RoutingType::REDIRECT;
#if I6E_PLATFORM == I6E_PLATFORM_WIN32
				sendCtrlMsg_(newInfo, _root, ControlTarget::ROOT);
#elif I6E_PLATFORM == I6E_PLATFORM_LINUX
				BaseRouting<NetworkType>::sendCtrlMsg_(newInfo, _root, ControlTarget::ROOT);
#endif
			} else {
				_nodes.insert(self_);
			}
		}

		void setSelf(const typename NetworkType::Key & self) {
			self_ = self;
			if (self_ == _root) {
				_nodes.insert(self_);
			}
		}

		void setUnsubscriptionListener(const std::function<void(const typename NetworkType::Key)> & listener) {
			_removed_subscribereventlistener = listener;
		}

		// set the self subscription status
		void selfSubscribed(const bool b) {
			selfSubscribed_ = b;
		}

		// get the self subscribtion status
		bool selfSubscribed() const {
			return selfSubscribed_;
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
					msgType = message::NOTIFY;
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
					M2ETIS_LOG_ERROR("DirectBroadcastRouting", "configureRoutingInfo called with wrong action type");
			}
			return;
		}

		/**
		 * Returns a list of target nodes for the specified message type and algoinfo in regard to the node's
		 * position in the logic distribution tree.
		 *
		 * \param mtype the type of the message (SUBSCRIBE, UNSUBSCIBE, or PUBLISH)
		 * \param algoinfo only contains info if mytpe == PUBLISH
		 */
		KeyList getTargetNodes(const message::ActionType mtype, typename message::RoutingInfo<NetworkType>::Ptr routingInfo, typename NetworkType::Key &) const {
			/*
			 * All messages must be sent to root.
			 * Even if it's a publishmessage and root is subscribed, too.
			 * In deliver::publish, root will take care to deliver the message and spread it to the subscribers.
			 */
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			KeyList m;
			if (mtype == message::SUBSCRIBE) {
				// if we got control message with all available nodes, we have to send all of them our subscribe
				if (rInfo->action == message::RoutingInfo<NetworkType>::RoutingType::REDIRECT) {
					m = _newSubs;
					// clear list with new nodes because they aren't new any longer
					const_cast<DirectBroadcastRouting *>(this)->_newSubs.clear();
					return m;
				}

				// if we currently haven't any subscriber, so the first subscribe will be sent
				if (!_nodes.empty()) {
					m.resize(_nodes.size());
					std::copy(_nodes.begin(), _nodes.end(), m.begin());
				}

				m.erase(std::remove_if(m.begin(), m.end(), [this](const typename NetworkType::Key & other) {
					return self_ == other;
				}), m.end());
			} else if (mtype == message::UNSUBSCRIBE) {
				m.resize(_nodes.size());
				std::copy(_nodes.begin(), _nodes.end(), m.begin());

				m.erase(std::remove_if(m.begin(), m.end(), [this](const typename NetworkType::Key & other) {
					return self_ == other;
				}), m.end());
			} else if (mtype == message::PUBLISH) {
				assert(false); // there aren't publish messages sent in DirectBroadcast
			} else if (mtype == message::CONTROL) {
				if (self_ == _root) {
					for (auto n : _nodes) {
						if (n != _root) {
							m.push_back(n);
						}
					}
				} else {
					m.push_back(_root);
				}
			} else if (mtype == message::NOTIFY) {
				// notify messages will be sent to every subscriber being registered
				struct T {
					static typename NetworkType::Key get_from_pair(const TimePair & p) {
						return p.second;
					}
				};

				m.resize(_subscriber.size());
				std::transform(_subscriber.begin(), _subscriber.end(), m.begin(), T::get_from_pair);

				// I have to "send" me the message to, but only if I'm subscribed on this Tree
				if (selfSubscribed() && std::find(m.begin(), m.end(), self_) == m.end()) {
					m.push_back(self_);
				}
			} else {
				assert(false);
			}

			return m;
		}

		/**
		 * Handle a subscribe message
		 *
		 * \param sender the subscriber's key
		 * \param algoinfo the payload created by this algorithm at the subscriber's node
		 * \return an information unit, whether the message should be stopped or needs to be changed
		 */
		bool processSubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key &, message::ActionType &) override {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			bool found = false;
			typename TimeList::iterator iter;
			for (iter = _subscriber.begin(); iter != _subscriber.end(); ++iter) {
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
				_subscriber.push_back(std::make_pair(_pssi->clock_.getTime(), sender));
			}
			rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
            return true;
		}

		/**
		 * Handle a unsubscribe message
		 *
		 * \param sender the unsubscriber's key
		 * \param algoinfo the payload created by this algorithm at the unsubscriber's node
		 * \return an information unit, whether the message should be stopped or needs to be changed
		 */
		void processUnsubscribePayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key &, message::ActionType &) {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			struct T {
				static bool test(const typename NetworkType::Key & send, const TimePair & paar) {
					return paar.second == send;
				}
			};

			_subscriber.erase(std::remove_if(_subscriber.begin(), _subscriber.end(), std::bind(T::test, sender, std::placeholders::_1)), _subscriber.end());
			rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
		}

		/**
		 * Handle a publish message
		 *
		 * \param sender the publisher's key
		 * \param algoinfo the payload created by this algorithm at the publisher's node
		 * \return an information unit, whether the message should be spread out to subscribers ond if the
		 * current node is the root-node for that topic.
		 */
		void processPublishPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key &, typename NetworkType::Key &, message::ActionType &) {
			assert(false); // there musn't be publish messages
		}

		/**
		 * Handle a notify message
		 *
		 * \param sender the notifier's key
		 * \param algoinfo the payload created by this algorithm at the notify's node
		 * \return an information unit, whether the message should be spread out to subscribers ond if the
		 * current node is the root-node for that topic.
		 */
		void processNotifyPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key &, typename NetworkType::Key &, message::ActionType &) {
			routingInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			return;
		}

		void processControlPayload(typename message::RoutingInfo<NetworkType>::Ptr routingInfo, const typename NetworkType::Key & sender, typename NetworkType::Key &, message::ActionType & msgType) {
			typename RoutingInfoType::Ptr rInfo = cast(routingInfo);
			// message returned from RP
			if (rInfo->action == message::RoutingInfo<NetworkType>::RoutingType::REDIRECT) {
				if (self_ == _root) {
					// new node created, notify existing nodes with list of all existing nodes
					if (_nodes.insert(sender).second) {
						if (selfSubscribed_) {
							msgType = message::SUBSCRIBE;
							rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::REDIRECT;
							_newSubs.push_back(sender);
						} else {
							rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
						}
						typename RoutingInfoType::Ptr newInfo = boost::make_shared<RoutingInfoType>();
						newInfo->action = RoutingInfoType::RoutingType::REDIRECT;
						newInfo->_nodes = _nodes;
						BaseRouting<NetworkType>::sendCtrlMsg_(newInfo, _root, ControlTarget::ALL);
					}
				} else {
					_nodes = rInfo->_nodes;
					if (selfSubscribed_) {
						msgType = message::SUBSCRIBE;
						rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::REDIRECT;
						for (auto n : _nodes) {
							bool found = false;
							for (TimePair p : _subscriber) {
								if (p.second == n) {
									found = true;
									break;
								}
							}
							if (!found) {
								_newSubs.push_back(n);
							}
						}
						rInfo->_nodes.clear();
					} else {
						rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
					}
				}
				
			} else { // nothing to do, stop the workflow
				rInfo->action = message::RoutingInfo<NetworkType>::RoutingType::STOP;
			}
			return;
		}

	private:
		// The topic name
		const unsigned short topic_name_;
		// The own key of this actual node
		typename NetworkType::Key self_;
		// Is this node subscribed on the topic?
		bool selfSubscribed_;

		// List of all subscribers (TimeStamp and Key)
		TimeList _subscriber;

		// Control variables for the purging thread
		volatile bool _purging;
		std::vector<typename NetworkType::Key> _newSubs;
		KeySet _nodes;

		typename NetworkType::Key _root;

		typename RoutingInfoType::Ptr cast(typename message::RoutingInfo<NetworkType>::Ptr ptr) const {
			typename RoutingInfoType::Ptr ret = boost::dynamic_pointer_cast<RoutingInfoType>(ptr);
			if (!ret) {
				M2ETIS_LOG_ERROR("DirectBroadcastRouting", "Downcast error of routingInfo");
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

			if (_subscriber.empty()) {
				return _purging;
			}

			// lock the list
			// get the current timestamp
			// and remove every subscriber which hasn't resubscribed within the "purge_distance"
			uint64_t jetzt = _pssi->clock_.getTime();

			auto iter_first_erased_subscriber = std::remove_if(_subscriber.begin(), _subscriber.end(), std::bind(T::test, purge_distance_, jetzt, std::placeholders::_1));
			// notify listeners about erased subscribers:
			for (auto iter_subscriber = iter_first_erased_subscriber; iter_subscriber != _subscriber.end(); ++iter_subscriber) {
				_removed_subscribereventlistener(iter_subscriber->second);
			}

			_subscriber.erase(iter_first_erased_subscriber, _subscriber.end());

			return _purging;
		}

		DirectBroadcastRouting & operator=(const DirectBroadcastRouting &);
	};

} /* namespace routing */
} /* namespace pubsub */
} /* namespace m2etis */

#endif /* __M2ETIS_PUBSUB_ROUTING_DIRECTBROADCASTROUTING_H__ */

/**
 * @}
 */
