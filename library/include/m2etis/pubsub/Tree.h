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

#ifndef __M2ETIS_PUBSUB_TREE_H__
#define __M2ETIS_PUBSUB_TREE_H__

#include "m2etis/util/Logger.h"

#include "m2etis/message/MessageFactory.h"
#include "m2etis/net/NetworkController.h"
#include "m2etis/net/NetworkFactory.h"
#include "m2etis/pubsub/ControlTargets.h"
#include "m2etis/pubsub/DeliverCallbackInterface.h"
#include "m2etis/pubsub/MessageBuffer.h"
#include "m2etis/pubsub/config/ChannelType.h"

#include "boost/bind.hpp"

#if I6E_PLATFORM == I6E_PLATFORM_WIN32
	#pragma warning(push)
	#pragma warning(disable : 4127)
#endif

namespace m2etis {
namespace pubsub {

	/**
	 * \class Tree
	 * \brief Tree
	 *
	 * Long Desc
	 *
	 */
	template<class ChannelType, class NetworkType, class EventType>
	class Tree : public ChannelType::RoutingStrategy, public ChannelType::FilterStrategy, public ChannelType::OrderStrategy, public ChannelType::DeliverStrategy, public ChannelType::PersistenceStrategy, public ChannelType::ValidityStrategy, public ChannelType::SecurityStrategy {
		typedef message::InternalMessage<NetworkType, ChannelType, EventType> IMessage;
		message::MessageFactory<ChannelType, NetworkType> factory_;
		uint16_t topic_;
		BasicDeliverCallbackInterface<EventType> * deliver_f;
		net::NetworkController<NetworkType> * controller_;
		typename NetworkType::Key self_;
		typename NetworkType::Key _rendezvous;
		typename NetworkType::Key _root;
		PubSubSystemEnvironment * pssi_;
		bool subscribed_;
		mutable MessageBuffer buffer_;
		int channelID_;
		uint64_t resubscribeID_;

	public:
		/**
		 * \brief Constructor
		 */
		Tree(uint16_t tree_name, const typename NetworkType::Key & self, const typename NetworkType::Key & rendezvous, const typename NetworkType::Key & root, PubSubSystemEnvironment * pssi, int cI) : ChannelType::RoutingStrategy(tree_name, pssi, root), ChannelType::OrderStrategy(pssi, false), ChannelType::DeliverStrategy(pssi, self), ChannelType::ValidityStrategy(pssi), factory_(message::MessageFactory<ChannelType, NetworkType>()), topic_(tree_name), deliver_f(), controller_(pssi->_factory->createNetworkController(NetworkType())), self_(self), _rendezvous(rendezvous), _root(root), pssi_(pssi), subscribed_(false), buffer_(), channelID_(cI), resubscribeID_(UINT64_MAX) {
			ChannelType::RoutingStrategy::setSelf(self_);
			ChannelType::FilterStrategy::setSelf(self_);
			ChannelType::OrderStrategy::setRoot(self_ == _rendezvous); // if this node is the RP, then set true flag to make this node sequenzer

			ChannelType::RoutingStrategy::setUnsubscriptionListener(std::bind(&Tree::processRoutingStrategyUnsubscribeNotification, this, std::placeholders::_1));

			ChannelType::RoutingStrategy::configureSendCallback(std::bind(&Tree::sendRoutingControlMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			ChannelType::DeliverStrategy::configureCallback(std::bind(&MessageBuffer::deliver, &buffer_, std::placeholders::_1, std::placeholders::_2));
			ChannelType::DeliverStrategy::configureSendCallback(std::bind(&Tree::sendDeliverControlMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			ChannelType::OrderStrategy::configureCallback(std::bind(&MessageBuffer::deliver, &buffer_, std::placeholders::_1, std::placeholders::_2));
			ChannelType::OrderStrategy::configureSendCallback(std::bind(&Tree::sendOrderControlMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			ChannelType::OrderStrategy::setHn(self_);
			registerMessageTypes();
			registerNetworkCallbacks();
		}

		virtual ~Tree() {
			if (resubscribeID_ != UINT64_MAX) {
				pssi_->scheduler_.stop(resubscribeID_);
				resubscribeID_ = UINT64_MAX;
			}
			deregisterNetworkCallbacks();
			deliver_f = nullptr;
		}

		inline typename NetworkType::Key getSelf() const {
			return controller_->getSelf();
		}

		inline typename NetworkType::Key getRoot() const {
			return _root;
		}

		inline unsigned short getChannel() const {
			return channelID_;
		}

		inline uint16_t getTopic() const {
			return topic_;
		}

		void subscribe(BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
			subscribe_impl(callback, predicate, Int2Type<ChannelType::RoutingStrategy::periodicSubscribtion>());
		}

		void publish(const typename message::M2Message<EventType>::Ptr msg) {
			typename IMessage::Ptr topublish = boost::static_pointer_cast<IMessage>(msg);
			message::ActionType tmp = message::PUBLISH;
			ChannelType::RoutingStrategy::configureRoutingInfo(tmp, topublish->routingInfo, topublish->receiver);
			ChannelType::ValidityStrategy::configureValidityInfo(topublish->validityInfo);
			topublish->type = tmp | topic_;
			sendMessages(topublish);
		}

		void unsubscribe() {
			if (subscribed_) {
				unsubscribe_impl(Int2Type<ChannelType::RoutingStrategy::periodicSubscribtion>());
				subscribed_ = false;
			}

			if (resubscribeID_ != UINT64_MAX) {
				pssi_->scheduler_.stop(resubscribeID_);
				resubscribeID_ = UINT64_MAX;
			}
		}

		// for filter strategies: deregistering single filter:
		void unsubscribe(boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
			if (subscribed_) {
				unsubscribe_impl(predicate);
				subscribed_ = false;
			}
		}

	private:
		void deliver(typename message::NetworkMessage<NetworkType>::Ptr msg) {
			// Security::decrypt(message);
			typename IMessage::Ptr todeliver = boost::static_pointer_cast<IMessage>(msg);

			if (todeliver->ctrlType_ == message::ControlType::ORDER) {
				if (ChannelType::DeliverStrategy::processOtherControlPayload(todeliver->deliverInfo, todeliver->sender)) {
					if (ChannelType::OrderStrategy::processControlPayload(todeliver->orderInfo, todeliver->sender)) {
						// deliver further down the tree
						sendMessages(todeliver);
					}
				}
				return;
			} else if (todeliver->ctrlType_ == message::ControlType::DELIVER) {
				if (ChannelType::DeliverStrategy::processControlPayload(todeliver->deliverInfo, todeliver->sender)) {
					// deliver further down the tree
					sendMessages(todeliver);
				}
				return;
			}

			message::ActionType tmp = static_cast<message::ActionType>(todeliver->type & message::ACTION_TYPE_MASK);

			switch(tmp) {
				case message::CONTROL: {
					if (ChannelType::DeliverStrategy::processOtherControlPayload(todeliver->deliverInfo, todeliver->sender)) {
						ChannelType::RoutingStrategy::processControlPayload(todeliver->routingInfo, todeliver->sender, todeliver->receiver, tmp);
						todeliver->type = tmp | topic_;
						disseminateMessage(todeliver);
					}
					break;
				}
				case message::SUBSCRIBE: {
					// give Routing and Filter the opportunity to update their internals
					if (ChannelType::DeliverStrategy::processSubscribePayload(todeliver->deliverInfo, todeliver->sender)) {
						if (ChannelType::RoutingStrategy::processSubscribePayload(todeliver->routingInfo, todeliver->sender, todeliver->receiver, tmp)) {
							ChannelType::OrderStrategy::processSubscribePayload(todeliver->orderInfo, todeliver->sender);

							ChannelType::FilterStrategy::processSubscribePayload(todeliver->sender, todeliver->filterInfo);
						}
						todeliver->type = tmp | topic_;
						// message type was changed to control here perhaps need for logic change in routing strategies
						disseminateMessage(todeliver);
					}
					break;
				}
				case message::UNSUBSCRIBE: {
					if (ChannelType::DeliverStrategy::processUnsubscribePayload(todeliver->deliverInfo, todeliver->sender)) {
						ChannelType::FilterStrategy::processUnsubscribePayload(todeliver->sender, todeliver->filterInfo);
						ChannelType::RoutingStrategy::processUnsubscribePayload(todeliver->routingInfo, todeliver->sender, todeliver->receiver, tmp);
						todeliver->type = tmp | topic_;
						disseminateMessage(todeliver);
					}
					break;
				}
				case message::PUBLISH: {
					ChannelType::RoutingStrategy::processPublishPayload(todeliver->routingInfo, todeliver->sender, todeliver->receiver, tmp);
					ChannelType::ValidityStrategy::processPublishPayload(todeliver->validityInfo);
					todeliver->type = tmp | topic_;
					if (ChannelType::DeliverStrategy::processPublishPayload(todeliver->deliverInfo, todeliver->sender)) {
						ChannelType::OrderStrategy::processPublishPayload(todeliver->orderInfo, todeliver->sender);
						if (todeliver->routingInfo->action != message::RoutingInfo<NetworkType>::RoutingType::STOP) {
							if (ChannelType::ValidityStrategy::isValid(todeliver->validityInfo)) {
								sendMessages(todeliver);
							}
						}
						if (ChannelType::RoutingStrategy::selfSubscribed()) {
							if (todeliver->sender == todeliver->receiver) {
								if (ChannelType::ValidityStrategy::isValid(todeliver->validityInfo)) {
									deliver_f->deliverCallback(*todeliver->payload);
								}
							} else {
								if (ChannelType::ValidityStrategy::isValid(todeliver->validityInfo)) {
									uint64_t a = buffer_.insert(std::bind(&BasicDeliverCallbackInterface<EventType>::deliverCallback, deliver_f, *todeliver->payload));
									ChannelType::OrderStrategy::receive(a, todeliver->orderInfo, todeliver->sender);
								}
							}
						}
					}
					break;
				}
				case message::NOTIFY: {
					ChannelType::RoutingStrategy::processNotifyPayload(todeliver->routingInfo, todeliver->sender, todeliver->receiver, tmp);
					ChannelType::ValidityStrategy::processNotifyPayload(todeliver->validityInfo);
					todeliver->type = tmp | topic_;
					if (ChannelType::DeliverStrategy::processNotifyPayload(todeliver->deliverInfo, todeliver->sender)) {
						ChannelType::OrderStrategy::processNotifyPayload(todeliver->orderInfo, todeliver->sender);
						if (todeliver->routingInfo->action != message::RoutingInfo<NetworkType>::RoutingType::STOP) {
							if (ChannelType::ValidityStrategy::isValid(todeliver->validityInfo)) {
								sendMessages(todeliver);
							} else {
								for (typename NetworkType::Key to : ChannelType::RoutingStrategy::getTargetNodes(tmp, todeliver->routingInfo, todeliver->receiver)) {
									ChannelType::OrderStrategy::notifyRemovedMessage(todeliver->orderInfo, to);
								}
							}
						}
						if (ChannelType::RoutingStrategy::selfSubscribed()) {
							if (ChannelType::FilterStrategy::match(todeliver->payload)) {
								if (todeliver->sender == todeliver->receiver) {
									if (ChannelType::ValidityStrategy::isValid(todeliver->validityInfo)) {
										deliver_f->deliverCallback(*todeliver->payload);
									}
								} else {
									if (ChannelType::ValidityStrategy::isValid(todeliver->validityInfo)) {
										uint64_t a = buffer_.insert(std::bind(&BasicDeliverCallbackInterface<EventType>::deliverCallback, deliver_f, *todeliver->payload));
										ChannelType::OrderStrategy::receive(a, todeliver->orderInfo, todeliver->sender);
									}
								}
							}
						}
					}
					break;
				}
				default: {
					M2ETIS_THROW_FAILURE("Tree", "deliver called with unknown action type", 1);
					break;
				}
			}
		}

		void registerMessageTypes() const {
			// Register each message type for the tree (calculating from type and treeID)
			controller_->registerMessageType(message::SUBSCRIBE | topic_);
			controller_->registerMessageType(message::UNSUBSCRIBE | topic_);
			controller_->registerMessageType(message::PUBLISH | topic_);
			controller_->registerMessageType(message::NOTIFY | topic_);
			controller_->registerMessageType(message::CONTROL | topic_);
		}

		void registerNetworkCallbacks() {
			typename net::NetworkController<NetworkType>::net_deliver_func deliv = std::bind(&Tree::deliver, this, std::placeholders::_1);
			typename net::NetworkController<NetworkType>::net_forward_func forw = std::bind(&Tree::forward, this, std::placeholders::_1);

			if (ChannelType::RoutingStrategy::register_forward_subscribe) {
				controller_->register_forward(message::SUBSCRIBE | topic_, forw);
			}
			if (ChannelType::RoutingStrategy::register_deliver_subscribe) {
				controller_->register_deliver(message::SUBSCRIBE | topic_, deliv);
			}
			if (ChannelType::RoutingStrategy::register_forward_unsubscribe) {
				controller_->register_forward(message::UNSUBSCRIBE | topic_, forw);
			}
			if (ChannelType::RoutingStrategy::register_deliver_unsubscribe) {
				controller_->register_deliver(message::UNSUBSCRIBE | topic_, deliv);
			}

			controller_->register_deliver(message::PUBLISH | topic_, deliv);
			controller_->register_deliver(message::NOTIFY | topic_, deliv);
			controller_->register_deliver(message::CONTROL | topic_, deliv);
		}

		void deregisterNetworkCallbacks() {
			/* look at http://www.boost.org/doc/libs/1_43_0/doc/html/function/tutorial.html#id866854
			 * for more information about binding a member method
			 * However, I'm using std::bind here! (It's less clumsy.)
			 */
			if (ChannelType::RoutingStrategy::register_forward_subscribe) {
				controller_->deregister_forward(message::SUBSCRIBE | topic_);
			}
			if (ChannelType::RoutingStrategy::register_deliver_subscribe) {
				controller_->deregister_deliver(message::SUBSCRIBE | topic_);
			}
			if (ChannelType::RoutingStrategy::register_forward_unsubscribe) {
				controller_->deregister_forward(message::UNSUBSCRIBE | topic_);
			}
			if (ChannelType::RoutingStrategy::register_deliver_unsubscribe) {
				controller_->deregister_deliver(message::UNSUBSCRIBE | topic_);
			}

			controller_->deregister_deliver(message::PUBLISH | topic_);
			controller_->deregister_deliver(message::NOTIFY | topic_);
			controller_->deregister_deliver(message::CONTROL | topic_);
		}

		void disseminateMessage(typename IMessage::Ptr msg)  {
			if (msg->routingInfo->action != message::RoutingInfo<NetworkType>::RoutingType::STOP) {
				sendMessages(msg);
			}
		}

		void sendDirect(typename IMessage::Ptr msg) {
			uint64_t a = buffer_.insert(std::bind(&Tree::reallySendMsg, this, msg));
			message::ActionType action = message::ActionType(msg->type & message::ACTION_TYPE_MASK);
			ChannelType::OrderStrategy::configureOrderInfo(a, action, msg->orderInfo, msg->receiver);
		}

		void sendMessages(typename IMessage::Ptr msg)  {
			message::ActionType action = message::ActionType(msg->type & message::ACTION_TYPE_MASK);

			// Algos remember: Publish is really handled in deliver!
			// Even if you're "Root" and selfsubscribed, send that message to _yourself_
			// because selfsubscribtion and "spread of message" is handled in deliver::publish!
			std::vector<typename NetworkType::Key> v = ChannelType::RoutingStrategy::getTargetNodes(action, msg->routingInfo, msg->receiver);
			// Build NetworkHeader from routingInfo
			msg->sender = self_;
			msg->type = action | topic_;

			size_t receiverSize = v.size();
			for (size_t i = 1; i < receiverSize; i++) {
				typename NetworkType::Key to = v[i];
				// Using the same Message and sending it to all receivers. Don't change it later!
				msg->receiver = to;

				if (action == message::NOTIFY && !ChannelType::FilterStrategy::match(to, msg->filterInfo, msg->payload)) {
					ChannelType::OrderStrategy::notifyRemovedMessage(msg->orderInfo, to);
					continue;
				}
				if ((action == message::NOTIFY || action == message::PUBLISH) && !ChannelType::ValidityStrategy::isValid(msg->validityInfo)) {
					ChannelType::OrderStrategy::notifyRemovedMessage(msg->orderInfo, to);
					continue;
				}
				typename IMessage::Ptr msg2 = boost::make_shared<IMessage>(*msg);
				uint64_t a = buffer_.insert(std::bind(&Tree::reallySendMsg, this, msg2));
				ChannelType::OrderStrategy::configureOrderInfo(a, action, msg2->orderInfo, to);
			}

			if (!v.empty()) {
				typename NetworkType::Key to = v[0];
				// Using the same Message and sending it to all receivers. Don't change it later!
				msg->receiver = to;

				if (action == message::NOTIFY && !ChannelType::FilterStrategy::match(to, msg->filterInfo, msg->payload)) {
					ChannelType::OrderStrategy::notifyRemovedMessage(msg->orderInfo, to);
					return;
				}
				if ((action == message::NOTIFY || action == message::PUBLISH) && !ChannelType::ValidityStrategy::isValid(msg->validityInfo)) {
					ChannelType::OrderStrategy::notifyRemovedMessage(msg->orderInfo, to);
					return;
				}
				uint64_t a = buffer_.insert(std::bind(&Tree::reallySendMsg, this, msg));
				ChannelType::OrderStrategy::configureOrderInfo(a, action, msg->orderInfo, to);
			}
		}

		void reallySendMsg(typename IMessage::Ptr msg) {
			message::ActionType action = message::ActionType(msg->type & message::ACTION_TYPE_MASK);
			uint64_t a = buffer_.insert(std::bind(&net::NetworkController<NetworkType>::send, controller_, boost::static_pointer_cast<message::NetworkMessage<NetworkType>>(msg)));
			ChannelType::DeliverStrategy::configureDeliverInfo(a, action, msg->deliverInfo, msg->receiver, msg->ctrlType_);
		}

		void sendControlMessage(typename IMessage::Ptr msg, const typename NetworkType::Key & receiver, ControlTarget target) {
			msg->sender = self_;
			msg->type = message::CONTROL | topic_;

			if (target == ControlTarget::ROOT) {
				msg->receiver = _root;
				sendDirect(msg);
			} else if (target == ControlTarget::ALL) {
				message::ActionType tmp = message::CONTROL;
				ChannelType::RoutingStrategy::configureRoutingInfo(tmp, msg->routingInfo, msg->receiver);
				msg->type = tmp | topic_;
				sendMessages(msg);
			} else if (target == ControlTarget::SINGLE) {
				msg->receiver = receiver;
				sendDirect(msg);
			} else {
				assert(false && "ControlTarget unknown");
			}
		}

		void sendDeliverControlMessage(message::DeliverInfo::Ptr p, const typename NetworkType::Key & receiver, ControlTarget target) {
#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = channelID_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>());
#endif /* WITH_SIM */

			msg->ctrlType_ = message::ControlType::DELIVER;
			msg->deliverInfo = boost::static_pointer_cast<typename ChannelType::DeliverStrategy::DeliverInfoType>(p);

			sendControlMessage(msg, receiver, target);
		}

		void sendOrderControlMessage(message::OrderInfo::Ptr p, const typename NetworkType::Key & receiver, ControlTarget target) {
#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = channelID_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>());
#endif /* WITH_SIM */

			msg->ctrlType_ = message::ControlType::ORDER;
			msg->orderInfo = boost::static_pointer_cast<typename ChannelType::OrderStrategy::OrderInfoType>(p);

			sendControlMessage(msg, receiver, target);
		}

		void sendRoutingControlMessage(typename message::RoutingInfo<NetworkType>::Ptr p, const typename NetworkType::Key & receiver, ControlTarget target) {
#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = channelID_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>());
#endif /* WITH_SIM */

			msg->ctrlType_ = message::ControlType::ROUTING;
			msg->routingInfo = boost::static_pointer_cast<typename ChannelType::RoutingStrategy::RoutingInfoType>(p);

			sendControlMessage(msg, receiver, target);
		}

		void subscribe_impl(BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
			deliver_f = &callback;

#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = channelID_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>());
#endif /* WITH_SIM */

			ChannelType::RoutingStrategy::selfSubscribed(true);
			message::ActionType tmp = message::SUBSCRIBE;
			ChannelType::RoutingStrategy::configureRoutingInfo(tmp, msg->routingInfo, msg->receiver);
			msg->type = tmp | topic_;

			ChannelType::FilterStrategy::getSubscribePayload(predicate, subscribed_, msg->filterInfo);

			sendMessages(msg);
		}

		bool subs(BasicDeliverCallbackInterface<EventType> * callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
			if (subscribed_) {
				subscribe_impl(*callback, predicate);
				return true;
			} else {
				return false;
			}
		}

		void subscribe_impl(BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate, Int2Type<true>) {
			subscribe_impl(callback, predicate);
			if (resubscribeID_ != UINT64_MAX) {
				pssi_->scheduler_.stop(resubscribeID_);
			}
			resubscribeID_ = pssi_->scheduler_.runRepeated(ChannelType::RoutingStrategy::periodic_, std::bind(&Tree::subs, this, &callback, predicate), 5);
			subscribed_ = true;
		}

		void subscribe_impl(BasicDeliverCallbackInterface<EventType> & callback, boost::shared_ptr<filter::FilterExp<EventType> > predicate, Int2Type<false>) {
			subscribe_impl(callback, predicate);
			subscribed_ = true;
		}

		void unsubscribe_impl() {
			subscribed_ = false;

#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = channelID_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>());
#endif /* WITH_SIM */

			deliver_f = nullptr;
			ChannelType::RoutingStrategy::selfSubscribed(false);
			message::ActionType tmp = message::UNSUBSCRIBE;
			ChannelType::RoutingStrategy::configureRoutingInfo(tmp, msg->routingInfo, msg->receiver);
			msg->type = tmp | topic_;
			ChannelType::FilterStrategy::getUnsubscribePayload(msg->filterInfo);
			sendMessages(msg);
		}

		// for filter strategies: deregistering single filter:
		void unsubscribe_impl(boost::shared_ptr<filter::FilterExp<EventType> > predicate) {
#ifdef WITH_SIM
			SimulationEventType v;
			v._simID = -1;
			v._simChannel = channelID_;
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>(v));
#else
			typename IMessage::Ptr msg = boost::static_pointer_cast<IMessage>(factory_.template createMessage<EventType>());
#endif /* WITH_SIM */

			typename ChannelType::FilterStrategy::FilterUnsubscribeInformation unsubscribeAction = ChannelType::FilterStrategy::getUnsubscribePayload(msg->filterInfo, predicate);

			if (unsubscribeAction == ChannelType::FilterStrategy::CANCELUNSUBSCRIBE) {
				// a subscriber of this node has registered the same filter,
				// so do not forward the filter
				return;
			}

			if (unsubscribeAction == ChannelType::FilterStrategy::UNSUBSCRIBEFROMTREE) {
				// the last filter has been deregistered => unsubscribe from the tree
				unsubscribe_impl();
			}

			if (unsubscribeAction == ChannelType::FilterStrategy::FORWARDUNSUBSCRIBE) {
				// => forward unsubscribe, but dont unsubscribe from tree
				// prevent routing strategies from unsubscribing from the tree by
				// turning unsubscription of single filter into subscription
				message::ActionType tmp = message::UNSUBSCRIBE;
				ChannelType::RoutingStrategy::configureRoutingInfo(tmp, msg->routingInfo, msg->receiver);
				msg->type = tmp | topic_;
				sendMessages(msg);
			}
		}

		void unsubscribe_impl(Int2Type<true>) {
			subscribed_ = false;
			unsubscribe_impl();
		}

		void unsubscribe_impl(Int2Type<false>) {
			subscribed_ = false;
			unsubscribe_impl();
		}

		// Callback Methods from Network
		FIPtr forward(typename message::NetworkMessage<NetworkType>::Ptr msg) {
			typename IMessage::Ptr toforward = boost::static_pointer_cast<IMessage>(msg);

			message::ActionType tmp = message::ActionType(toforward->type & message::ACTION_TYPE_MASK);

			if ((toforward->type & message::MSG_TYPE_MASK) != topic_) {
				M2ETIS_THROW_FAILURE("Tree", "Tree got message for forwarding with incorrect channel topic", 1);
			}
			// Security::decrypt(message);
			bool stop = false;
			bool changes = false;
			switch(tmp) {
				case message::SUBSCRIBE: {
					// Give Algo and Filter the oportunity to update their internals.
					// Perhaps the Filter wants to merge the predicates?!
					if (ChannelType::RoutingStrategy::processSubscribePayload(toforward->routingInfo, toforward->sender, toforward->receiver, tmp)) {
						std::string filterinfo = ChannelType::FilterStrategy::processSubscribePayload(toforward->sender, toforward->filterInfo);
						// FIXME what if Filter created a generalized version of the filter? How is this communicated to other nodes up in the tree?
					}

					if (toforward->routingInfo->action != message::RoutingInfo<NetworkType>::RoutingType::STOP) {
						changes = true;
						msg->sender = self_;
					} else {
						stop = true;
					}
					break;
				}
				case message::UNSUBSCRIBE: {
					ChannelType::FilterStrategy::processUnsubscribePayload(toforward->sender, toforward->filterInfo);
					ChannelType::RoutingStrategy::processUnsubscribePayload(toforward->routingInfo, toforward->sender, toforward->receiver, tmp);
					break;
				}
				default: {
					M2ETIS_THROW_FAILURE("Tree", "Tree got message for forwarding with unknown action type", 1);
					break;
				}
			}

			toforward->type = tmp | topic_;

			return boost::make_shared<ForwardInfo>(stop, changes);
		}
	};

} /* namespace pubsub */
} /* namespace m2etis */

#if I6E_PLATFORM == I6E_PLATFORM_WIN32
	#pragma warning(pop)
#endif

#endif /* __M2ETIS_PUBSUB_TREE_H__ */

/**
 * @}
 */
